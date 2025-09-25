#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "data_store_config.h"
#include "data_store_queue.h"
#include "data_store_history.h"
#include "data_store_event.h"
#include "data_store_wave.h"
#include "data_store_private.h"
#include "data_store_port.h"
#include "data_store.h"
#include "data_store_debug.h"



int8_t data_store_refresh_directory(struct Record_Context *rc)
{
    if(rc->dir_erase)
    {
        rc->dir_erase = false;
        data_store_port_erase_flash(rc->cr.dir_flag_block_start/FLASH_BLOCK_SIZE, 1);        // directory flag region alway one block
        data_store_port_write_flash(rc->cr.dir_flag_block_start/FLASH_BLOCK_SIZE, 0, 0, (uint8_t *)rc, sizeof(struct Record_Context));
        data_store_port_erase_flash(rc->cr.dir_bit_block_start/FLASH_BLOCK_SIZE, rc->cr.dir_bit_block_len);
    }

    uint32_t pages = (rc->wp.block_index - rc->cr.content_block_start)*PAGES_PER_BLOCK + rc->wp.page_index;
    uint32_t msg_per_page = FLASH_PAGE_SIZE/rc->msg_size;
    uint32_t entries = pages*msg_per_page + rc->wp.offset/rc->msg_size; // entry must greater than 0
    uint32_t dir_bytes = entries/8 + ((entries%8) ? 1 : 0);

    uint32_t dir_pages = dir_bytes/FLASH_PAGE_SIZE + ((dir_bytes%FLASH_PAGE_SIZE) ? 1 : 0);
    uint32_t dir_blocks = dir_pages/PAGES_PER_BLOCK + ((dir_pages%PAGES_PER_BLOCK) ? 1 : 0);

    uint32_t dir_block_index = rc->cr.dir_bit_block_start + dir_blocks-1;
    uint32_t dir_page_index = dir_pages-1;
    uint32_t dir_page_offset = dir_bytes % FLASH_PAGE_SIZE-1;
    

    uint8_t rbf[FLASH_PAGE_SIZE];
    memset(rbf, 0, sizeof(rbf));
    data_store_port_read_flash(dir_block_index, dir_page_index, 0, rbf, FLASH_PAGE_SIZE);
    for (uint32_t i = 0; i < dir_page_offset; i++)
    {
        if(rbf[i] != 0x00){
            return -1;  
        }
    }
    if(rbf[dir_page_offset] & (1 << ((entries-1)%8)))           // check the bit must be 1(not been writen yet)
    {
        rbf[dir_page_offset] &= ~(1 << ((entries-1)%8));        // clear the bit to 0(written)
    }else{
        return -1;                                      // already written(report error)
    }
    // write back to flash
    int8_t ret = data_store_port_write_flash(dir_block_index, dir_page_index, 0, rbf, FLASH_PAGE_SIZE);


    return ret;
}




/**
 * @brief  flash option cycle
 * @param  start_block_index: the start block index of the region
 * @param  end_block_index: the end block index of the region
 * @param  buf: if buf is NOT NULL, and read/write buf to/from the block, erase the block between in and out position(if needed)
 * @param  bytes: positive value means forward operation, negative value means backward operation
 * @param  operation_type: 1-write, 2-read, 
 * @note these is a little error catching in this function, need to be improved in many ports
 *
 */
static int8_t flash_operation_cycle(uint32_t start_block_index, 
                               uint32_t end_block_index, 
                               uint8_t *buf,                             
                               int32_t bytes, 
                               uint8_t operation_type,
                               struct Operate_Position *in, 
                               struct Operate_Position *out)
{
    uint32_t abs_bytes = bytes > 0 ? (uint32_t)bytes : (uint32_t)(-bytes);
    if(abs_bytes > (end_block_index - start_block_index -2)*FLASH_BLOCK_SIZE) {
        return -1;
    }
    memcpy(out, in, sizeof(struct Operate_Position));
    uint32_t pages = abs_bytes % FLASH_BLOCK_SIZE / FLASH_PAGE_SIZE;
    uint32_t offset = abs_bytes % FLASH_PAGE_SIZE;  
    uint32_t blocks = abs_bytes / FLASH_BLOCK_SIZE;

    if(bytes > 0){                  // forward
        out->offset += offset;
        if(out->offset >= FLASH_PAGE_SIZE) {
            pages++;
            out->offset = out->offset - FLASH_PAGE_SIZE;
        }
        out->page_index += pages;
        if(out->page_index >= PAGES_PER_BLOCK) {
            blocks++;
            out->page_index = out->page_index - PAGES_PER_BLOCK;
        }
        out->block_index += blocks;

        if(out->block_index >= end_block_index) {
            out->block_index = out->block_index - end_block_index + start_block_index;
            if(buf){            // Oops! We need to judge if an error occurs here (in many ports erase/write operation).
                    uint32_t write_count = 0;
                    write_count += FLASH_PAGE_SIZE - in->offset;
                    write_count += (PAGES_PER_BLOCK - in->page_index - 1)*FLASH_PAGE_SIZE;
                    write_count += (end_block_index - in->block_index -1)*FLASH_BLOCK_SIZE;
                if(operation_type == 1){            // write
                    data_store_port_erase_flash(in->block_index, end_block_index-in->block_index);      // erase to region end
                    data_store_port_write_flash(in->block_index, in->page_index, in->offset, buf, write_count);
                    data_store_port_erase_flash(start_block_index, out->block_index-start_block_index);
                    data_store_port_write_flash(start_block_index, 0, in->offset, buf+write_count, (uint32_t)bytes-write_count);
                }else if(operation_type == 2){      // read
                    data_store_port_read_flash(in->block_index, in->page_index, in->offset, buf, write_count);
                    data_store_port_read_flash(start_block_index, 0, in->offset, buf+write_count, (uint32_t)bytes-write_count);
                }
            }
        }else{
            if(buf){
                if(operation_type == 1){        // write
                    data_store_port_write_flash(in->block_index, in->page_index, in->offset, buf,  (uint32_t)bytes);
                    if(out->block_index != in->block_index)
                    {
                        data_store_port_erase_flash(in->block_index+1, out->block_index - in->block_index);     // in->block_index is already ready to write(erased before)
                    }
                }else if(operation_type == 2){  // read
                    data_store_port_read_flash(in->block_index, in->page_index, in->offset, buf,  (uint32_t)bytes);
                }
            }
        }
    }else{
        if(out->offset < offset) {
            out->offset = FLASH_PAGE_SIZE - (offset - out->offset);
            pages++;
            if(pages == PAGES_PER_BLOCK)
            {
                blocks++;
                pages = 0;
            }
        }else{
            out->offset -= offset;
        }

        if(out->page_index < pages)
        {
            out->page_index = PAGES_PER_BLOCK - (pages - out->page_index);
            blocks++;
        }else{
            out->page_index -= pages;

        }
        
        if(out->block_index < blocks + start_block_index)
        {
            out->block_index = end_block_index - (blocks + start_block_index - out->block_index);
        }else{
            out->block_index -= blocks;
        }
    }

    return 0;
}



/**
 * @note the implementation of this function is ugly, need to be optimized
 */
int8_t data_store_move(struct Record_Context *rc, int32_t move_entries, struct Operate_Position *out)
{
    int8_t ret = 0;
    int32_t bytes = move_entries * rc->msg_size;

    if(rc->cycle == 0 && bytes < 0)
    {
        // backward move lenth limited in the first cycle
        if((rc->wp.block_index - rc->cr.content_block_start)*FLASH_BLOCK_SIZE + rc->wp.page_index*FLASH_PAGE_SIZE + rc->wp.offset < (uint32_t)(-bytes))
        {
            return -1;
        }
    }

    ret = flash_operation_cycle(rc->cr.content_block_start, rc->cr.content_block_end, NULL, (int32_t)bytes, 1, &rc->wp, out);
    return ret;
}


uint32_t data_store_count_messages(struct Record_Context *rc)
{
    uint32_t msgs = 0;
    uint32_t bytes = 0;
    if(rc->cycle == 0)
    {
        bytes = (rc->wp.block_index - rc->cr.content_block_start)*FLASH_BLOCK_SIZE + rc->wp.page_index*FLASH_PAGE_SIZE + rc->wp.offset;
    }else{
        bytes = (rc->cr.content_block_end - rc->cr.content_block_start -2)*FLASH_BLOCK_SIZE;
    }
    msgs = bytes/rc->msg_size;
    return msgs;
}





int8_t data_store_read_message(struct Record_Context *rc, struct Operate_Position *op, struct Data_Store_Message *msg)
{

    int8_t ret = 0;
    struct Operate_Position out;
    ret = flash_operation_cycle(rc->cr.content_block_start, rc->cr.content_block_end, msg->buf, (int32_t)msg->size, 2, op, &out);
    if(ret != 0) {
        return ret;    // move error
    }


    return 0;




}



int8_t data_store_write_message(struct Record_Context *rc, struct Data_Store_Message *msg)
{

    int8_t ret = 0;
    // rewrite message(fill the record id feild)        len = MESSAGE_HEADER_SIZE
    memcpy(msg->buf, &rc->record_id, sizeof(rc->record_id));
    struct Operate_Position out;
    ret = flash_operation_cycle(rc->cr.content_block_start, rc->cr.content_block_end, msg->buf, (int32_t)msg->size, 1, &rc->wp, &out);
    if(ret != 0) {
        return ret;    // move error
    }
    // update write position
    if(out.block_index < rc->wp.block_index)          // cycle write happend
    {
        rc->cycle += 1;
        rc->dir_erase = true;                           // action in data_store_refresh_directory function( call afeter this function)
    }

    memcpy(&rc->wp, &out, sizeof(struct Operate_Position));

    // update read position if needed 
    if(rc->cycle > 0)                                   
    {
        rc->rp.block_index = rc->wp.block_index + 1;
        if(rc->rp.block_index >= rc->cr.content_block_end)
        {
            rc->rp.block_index = rc->cr.content_block_start;
        }
        rc->rp.page_index = 0;
        rc->rp.offset = 0;
    }

    
    rc->record_id += 1;

    return 0;
}



int8_t data_store_init_directory_from_flash(struct Record_Context *rc)
{

    struct Record_Context flash_rc;
    memset(&flash_rc, 0, sizeof(flash_rc));
    data_store_port_read_flash(rc->cr.dir_flag_block_start, 0, 0, (uint8_t *)&flash_rc, sizeof(flash_rc));
    
    struct Record_Context flash_rc_new_flash;
    memset(&flash_rc_new_flash, 0xff, sizeof(flash_rc_new_flash));
    if(memcmp(&flash_rc, &flash_rc_new_flash, sizeof(flash_rc)) == 0)
    {
        DEBUG_LOG("this is a new history directory region\n");
        // reset to new flash region
        rc->wp.block_index = rc->cr.content_block_start;
        rc->wp.page_index = 0;
        rc->wp.offset = 0;
        rc->rp.block_index = rc->cr.content_block_start;
        rc->rp.page_index = 0;
        rc->rp.offset = 0;
        rc->cycle = 0;
        data_store_port_write_flash(rc->cr.dir_flag_block_start/FLASH_BLOCK_SIZE, 0, 0, (uint8_t *)rc, sizeof(struct Record_Context));

        return 0;
    }

    bool overwrite = false;
    if(flash_rc.cycle>0)
    {
        overwrite = true;
    }
    rc->cycle = flash_rc.cycle;
    uint32_t bit_region_pages = rc->cr.dir_bit_block_len * PAGES_PER_BLOCK;
    uint8_t read_page_buf[FLASH_PAGE_SIZE];
    uint32_t zerobits = 0, i = 0, j = 0;
    for(; i < bit_region_pages; i++)
    {
        memset(read_page_buf, 0xff, sizeof(read_page_buf));
        data_store_port_read_flash(rc->cr.dir_bit_block_start, 0, 0, read_page_buf, sizeof(read_page_buf));
        for(; j < FLASH_PAGE_SIZE; j++)
        {
            if(read_page_buf[j] != 0){
                goto PARSE_END;
            }
        }
    }

PARSE_END:
    zerobits = (i*FLASH_PAGE_SIZE + j)*8;
    for(uint32_t k = 0; k < 8; k++)
    {
        if((read_page_buf[j] & (1<<k)) == 0)
        {
            zerobits++;
        }
    }
    DEBUG_LOG("bit region parse end, zero bits is %d\n", zerobits);
    uint32_t msg_per_page = FLASH_PAGE_SIZE/rc->msg_size;
    uint32_t page_idx = zerobits/msg_per_page;
    uint32_t block_idx = page_idx/PAGES_PER_BLOCK;
    rc->wp.block_index = rc->cr.content_block_start + block_idx;

    if (rc->wp.block_index < rc->cr.content_block_start 
        || rc->wp.block_index > rc->cr.content_block_end)  // error range
    { 
        // reset to new flash region
        rc->wp.block_index = rc->cr.content_block_start;
        rc->wp.page_index = 0;
        rc->wp.offset = 0;
        rc->rp.block_index = rc->cr.content_block_start;
        rc->rp.page_index = 0;
        rc->rp.offset = 0;
        rc->cycle = 0;
        return -1;
    }
    rc->wp.page_index = page_idx%PAGES_PER_BLOCK;
    rc->wp.offset = zerobits%msg_per_page * rc->msg_size;
    if(overwrite)
    {
        rc->rp.block_index = rc->wp.block_index;
        if(rc->rp.block_index >= rc->cr.content_block_end)
        {
            rc->rp.block_index = rc->cr.content_block_start;
        }
    }else{
        rc->rp.block_index = rc->cr.content_block_start;
    }
    rc->rp.page_index = 0;
    rc->rp.offset = 0;


    return 0;

}










void data_store_init(void)
{

    data_store_history_init();

    data_store_event_init();

    data_store_wave_init();



}









void data_store_task(void)
{

    data_store_history_task();

    data_store_event_task();

    data_store_wave_task();

}











