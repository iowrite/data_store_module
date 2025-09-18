#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "data_store_config.h"
#include "data_store_queue.h"
#include "data_store_private.h"
#include "data_store_port.h"
#include "data_store_debug.h"

struct History_Record{
    struct Record_Context rc;
    /* member for history itself */

};
struct History_Record g_history_record;

struct Message_History
{
    struct Data_Store_Message header;
    uint8_t buf[HISTORY_MSG_SIZE];
};


struct Queue_History g_queue_history = {
    .queue.buf = g_queue_history.buf,
    .queue.head = 0,
    .queue.tail = 0,
    .queue.size = sizeof(g_queue_history.buf)
};


int8_t data_store_write_history(struct Message_History* msg)
{
    // add lock here if needed

    data_store_write_message(&g_history_record.rc, &msg->header);
    // release lock here if needed

    // refresh directory
    data_store_refresh_directory(&g_history_record.rc);
    // write content
    


    return 0;
}



int8_t data_store_history_init(void)
{
    g_history_record.rc.cr.content_block_start = CONTENT_START_ADDR_HISTORY/FLASH_BLOCK_SIZE;
    g_history_record.rc.cr.content_block_end = (CONTENT_START_ADDR_HISTORY + CONTENT_LEN_HISTORY)/FLASH_BLOCK_SIZE;
    g_history_record.rc.cr.dir_flag_block_start = DIRECTORY_START_ADDR_FLAGS_HIS/FLASH_BLOCK_SIZE;
    g_history_record.rc.cr.dir_bit_block_start = DIRECTORY_START_ADDR_HISTORY/FLASH_BLOCK_SIZE;
    g_history_record.rc.cr.dir_bit_block_len = DIRECTORY_LEN_BIT_HIS/FLASH_BLOCK_SIZE;
    g_history_record.rc.msg_size = HISTORY_MSG_SIZE;
    g_history_record.rc.dir_erase = false;
    g_history_record.rc.record_id = 0;
    // need read directory from flash
    int8_t ret = data_store_init_directory_from_flash(&g_history_record.rc);
    if(ret != 0)
    {
        DEBUG_LOG("data_store_init_directory_from_flash failed\n");
        return ret;
    }

    return 0;
}



void data_store_history_task(void)
{
    
    struct Message_History msg;
    msg.header.buf = msg.buf;
    msg.header.size = sizeof(msg.buf);
    // we need a mux lock here if in rtos environment
    while(data_store_dequeue(&g_queue_history.queue, &msg.header) == msg.header.size)   // get a whole/integral frame of message
    {
        DEBUG_LOG("====> ----");
        // store to flash
        data_store_write_history(&msg);
        DEBUG_LOG("history write a message to flash\n");

    }
    // release lock
}

void data_store_post_a_history(uint8_t *msg, uint32_t len)
{

    struct Message_History m;
    memset(&m, 0, sizeof(m));
    if(len > (HISTORY_MSG_SIZE-MESSAGE_HEADER_SIZE)) {
        len = (HISTORY_MSG_SIZE-MESSAGE_HEADER_SIZE);   // truncate
    }
    memcpy(m.buf+MESSAGE_HEADER_SIZE, msg, len);
    m.header.buf = m.buf;
    m.header.size = (uint16_t)(len+MESSAGE_HEADER_SIZE);

    data_store_enqueue(&g_queue_history.queue, &m.header);
}


void data_store_record_a_history_blocking(uint8_t *msg, uint32_t len)
{

    struct Message_History m;
    memset(&m, 0, sizeof(m));
    if(len > (HISTORY_MSG_SIZE-MESSAGE_HEADER_SIZE)) {
        len = (HISTORY_MSG_SIZE-MESSAGE_HEADER_SIZE);   // truncate
    }
    memcpy(m.buf+MESSAGE_HEADER_SIZE, msg, len);
    m.header.buf = m.buf;
    m.header.size = (uint16_t)(len+MESSAGE_HEADER_SIZE);


    // store to flash
    data_store_write_history(&m);




}







