#include <stdint.h>
#include "data_store_private.h"


#ifndef __DATA_STORE_WAVE_H__
#define __DATA_STORE_WAVE_H__




struct Wave_Record{
    struct Record_Context rc;
    uint8_t msg_buf[2048*10];
    uint32_t buf_idx;
};




int8_t data_strore_wave_init(void);
void data_strore_wave_task(void);




#endif // __DATA_STORE_WAVE_H__