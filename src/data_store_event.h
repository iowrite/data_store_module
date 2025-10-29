#include <stdint.h>
#include "data_store_private.h"


#ifndef __DATA_STORE_EVENT_H__
#define __DATA_STORE_EVENT_H__




struct Event_Record{
    struct Record_Context rc;
    uint8_t msg_buf[2048];
    uint32_t buf_idx;
};


extern struct Event_Record g_event_record;

int8_t data_store_event_init(void);


void data_store_event_task(void);














#endif // __DATA_STORE_EVENT_H__
