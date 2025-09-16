#include <stdint.h>


#ifndef __DATA_STORE_H__
#define __DATA_STORE_H__



void data_strore_task(void);
void data_store_post_a_history(uint8_t *msg, uint32_t len);
void data_store_record_a_history_blocking(uint8_t *msg, uint32_t len);




#endif // __DATA_STORE_H__