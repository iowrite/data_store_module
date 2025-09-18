#include <stdint.h>
#include <stdbool.h>



#ifndef __DATA_STORE_PRIVATE_H__
#define __DATA_STORE_PRIVATE_H__


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


struct Data_Store_Message{
    uint8_t *buf;
    uint16_t size;
};

int8_t data_store_refresh_directory(struct Record_Context *rc);
int8_t data_store_write_message(struct Record_Context *rc, struct Data_Store_Message *msg);
int8_t data_store_init_directory_from_flash(struct Record_Context *rc);




#endif // __DATA_STORE_PRIVATE_H__