#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "data_store_config.h"
#include "data_store_queue.h"
#include "data_store_private.h"
#include "data_store_port.h"
#include "data_store_debug.h"
#include "data_store.h"




struct History_Record{
    struct Record_Context rc;
    /* member for history itself */
    bool exporting;

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
    g_history_record.exporting = false;
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







static struct Operate_Position history_summary_ary[MAX_STORE_SPAN];
static struct Record_Date history_summary_date_ary[MAX_STORE_SPAN];
struct Record_Summary s_record_summary_history = {
    .ary = history_summary_ary,
    .date_ary = history_summary_date_ary,
    .len = MAX_STORE_SPAN,
    .head = 0,
    .tail = 0
};



void data_store_history_task(void)
{
    if(!g_history_record.exporting){
        struct Message_History msg;
        msg.header.buf = msg.buf;
        msg.header.size = sizeof(msg.buf);
        // we **may** need a mux lock here if in rtos environment
        while(data_store_dequeue(&g_queue_history.queue, &msg.header) == msg.header.size)   // get a whole/integral frame of message
        {
            uint16_t now_year = 0;
            uint8_t now_month = 0;
            uint8_t now_day = 0;
            data_store_port_get_date(&now_year, &now_month, &now_day);
            uint32_t now_days = now_year*365 + now_month*30 + now_day;
            static uint32_t last_days = 0;
            if(now_days - last_days >= 1)   // once a day
            {
                last_days = now_days;
                DEBUG_LOG("history summary update");
                data_store_write_summary_entry(&s_record_summary_history, &g_history_record.rc.wp);
            }

            DEBUG_LOG("====> ----");
            // store to flash
            data_store_write_history(&msg);
            DEBUG_LOG("history write a message to flash\n");
        }
        // ^^^release lock here
    }
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
    if(!g_history_record.exporting){
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

}


void data_store_batch_export_history_start(void)
{
    g_history_record.exporting = true;
}
void data_store_batch_export_history_end(void)
{
    g_history_record.exporting = false;
}




uint32_t data_store_get_history_num(void)
{

    uint32_t msgs = data_store_count_messages(&g_history_record.rc);
    if(msgs > MAX_HISTROY_RECORD_NUM)
    {
        msgs = MAX_HISTROY_RECORD_NUM;
    }

    return msgs;

}


void data_store_get_history_summary(uint32_t *id_ary, struct Record_Date *date_ary)
{
    uint32_t raw_id_ary[MAX_STORE_SPAN];
    data_store_convert_summary_toidx(&s_record_summary_history, &g_history_record.rc, raw_id_ary);
    if(s_record_summary_history.head < s_record_summary_history.tail)
    {
        memcpy(id_ary, raw_id_ary, (uint32_t)s_record_summary_history.tail*sizeof(uint32_t));
        memcpy(date_ary, s_record_summary_history.date_ary, (uint32_t)s_record_summary_history.tail*sizeof(struct Record_Date));
    }else{
        uint32_t offset = (uint32_t)s_record_summary_history.head*sizeof(uint32_t); 
        uint32_t count = (uint32_t)s_record_summary_history.len*sizeof(uint32_t) - offset;
        memcpy(id_ary, raw_id_ary+offset, count);
        memcpy(id_ary+count, raw_id_ary, offset);
        uint32_t offset_date = (uint32_t)s_record_summary_history.head*sizeof(struct Record_Date); 
        uint32_t count_date = (uint32_t)s_record_summary_history.len*sizeof(struct Record_Date) - offset_date;
        memcpy(date_ary, s_record_summary_history.date_ary+offset_date, count_date);
        memcpy(date_ary+count_date, s_record_summary_history.date_ary, offset_date);
    }


    return;

}





/**
 * @brief get a record history
 * @note this function write a fix len to buf (HISTORY_MSG_SIZE)
 */
int8_t data_store_get_record_history(uint32_t idx, uint8_t *buf)
{
    uint32_t msgs = data_store_count_messages(&g_history_record.rc);
    if(msgs > MAX_HISTROY_RECORD_NUM)
    {
        msgs = MAX_HISTROY_RECORD_NUM;
    }

    idx -= 1;  // convert to 0 based index
    if(idx > msgs)
    {
        return -1;
    }
    idx= msgs - idx;   // convert to from the last record

    struct Operate_Position op;
    data_store_move(&g_history_record.rc, (int32_t)-idx, &op);
    struct Data_Store_Message msg = {buf, g_history_record.rc.msg_size};
    data_store_read_message(&g_history_record.rc, &op, &msg);


    return 0;

}




