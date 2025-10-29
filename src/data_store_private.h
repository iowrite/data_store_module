#include <stdint.h>
#include <stdbool.h>
#include "data_store_config.h"


#ifndef __DATA_STORE_PRIVATE_H__
#define __DATA_STORE_PRIVATE_H__

#if DS_CFG_P_OS
    #define DATA_STORE_MUTEX_LOCK()        data_store_port_mux_lock()
    #define DATA_STORE_MUTEX_UNLOCK()      data_store_port_mux_unlock()
#else
    #define DATA_STORE_MUTEX_LOCK()
    #define DATA_STORE_MUTEX_UNLOCK()
#endif


#define UNUSED(x) (void)(x)


struct Operate_Position {
    uint32_t block_index;
    uint32_t page_index;
    uint32_t offset;
};



struct Content_Region{
    uint32_t content_block_start;
    uint32_t content_block_end;
    uint32_t dir_flag_block_start;
    uint32_t dir_bit_block_start;
    uint32_t dir_bit_block_len;
};


struct Record_Context{
    struct Content_Region cr;       // content region
    struct Operate_Position wp;     // write position
    struct Operate_Position rp;     // read position
    bool dir_erase;                 // true if directory need to be erased
    uint32_t  cycle;                // cycle number         
    uint16_t msg_size;
    uint32_t record_id;
};







int8_t data_store_refresh_directory(struct Record_Context *rc);

int8_t data_store_init_directory_from_flash(struct Record_Context *rc);

int8_t data_store_move(struct Record_Context *rc, int32_t move_entries, struct Operate_Position *out);

uint32_t data_store_count_messages(struct Record_Context *rc);

struct Data_Store_Message{
    uint8_t *buf;
    uint16_t size;
};
int8_t data_store_read_message(struct Record_Context *rc, struct Operate_Position *op, struct Data_Store_Message *msg);
int8_t data_store_write_message(struct Record_Context *rc, struct Data_Store_Message *msg);






struct Record_Summary{
    struct Operate_Position *ary;
    struct Record_Date *date_ary;
    int len;
    int head;
    int tail;
};
void data_store_write_summary_entry(struct Record_Summary *summary, struct Operate_Position *pos);



void data_store_convert_summary_toidx(struct Record_Summary *summary, struct Record_Context *rc, uint32_t *idx_ary);

#endif // __DATA_STORE_PRIVATE_H__

