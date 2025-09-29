#include <stdint.h>


#ifndef __DATA_STORE_H__
#define __DATA_STORE_H__


struct Record_Date
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
};



void data_store_init(void);
void data_store_task(void);


void data_store_post_a_history(uint8_t *msg, uint32_t len);
void data_store_record_a_history_blocking(uint8_t *msg, uint32_t len);
uint32_t data_store_get_history_num(void);
int8_t  data_store_get_record_history(uint32_t idx, uint8_t *buf);
void data_store_batch_export_history_start(void);
void data_store_batch_export_history_end(void);
void data_store_get_history_summary(uint32_t *id_ary, struct Record_Date *date_ary);



#endif // __DATA_STORE_H__