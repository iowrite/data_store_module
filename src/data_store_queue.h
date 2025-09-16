#include <stdint.h>
#include <stdbool.h>
#include "data_store_config.h"
#include "data_store_private.h"

#define STATIC_IS_POWER_OF_TWO(n) (((n) & ((n) - 1)) == 0 && (n) > 0)


#ifndef __DTA_STORE_QUEUE_H__
#define __DTA_STORE_QUEUE_H__


struct Data_Store_Queue
{
    uint8_t *buf;
    uint32_t head;
    uint32_t tail;
    uint32_t size;
};

struct Queue_History
{
    struct Data_Store_Queue queue;
    uint8_t buf[DS_CFG_Q_HISTORY_SIZE];
#if !STATIC_IS_POWER_OF_TWO(DS_CFG_Q_HISTORY_SIZE)
    #error "DS_CFG_Q_HISTORY_SIZE must be power of 2"
#endif 
};


struct Queue_Event
{
    struct Data_Store_Queue queue;
    uint8_t buf[DS_CFG_Q_EVENT_SIZE];
#if !STATIC_IS_POWER_OF_TWO(DS_CFG_Q_EVENT_SIZE)
    #error "DS_CFG_Q_EVENT_SIZE must be power of 2"
#endif
};


struct Queue_Wave
{
    struct Data_Store_Queue queue;
    uint8_t buf[DS_CFG_Q_WAVE_SIZE];
#if !STATIC_IS_POWER_OF_TWO(DS_CFG_Q_WAVE_SIZE)
    #error "DS_CFG_Q_WAVE_SIZE must be power of 2"
#endif
};

extern struct Queue_History g_queue_history;
extern struct Queue_Event g_queue_event;
extern struct Queue_Wave g_queue_wave;




#define QUEUE_SIZE_MASK(q) ((q)->size - 1)
static inline bool data_store_queue_is_full(struct Data_Store_Queue *q)
{
    return ((q->tail - q->head) & QUEUE_SIZE_MASK(q)) == QUEUE_SIZE_MASK(q);
}

static inline uint8_t data_store_queue_is_empty(struct Data_Store_Queue *q) {
  return (q->head == q->tail);
}


void data_store_enqueue(struct Data_Store_Queue *q, struct Data_Store_Message *msg);

int32_t data_store_dequeue(struct Data_Store_Queue *q, struct Data_Store_Message *msg);





#endif // __DTA_STORE_QUEUE_H__