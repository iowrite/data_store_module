#include <stdbool.h>
#include "data_store_queue.h"
#include "data_store_private.h"
#include "data_store_debug.h"
#include "data_store_port.h"

#define QUEUE_SIZE_MASK(q) ((q)->size - 1)
bool data_store_queue_is_full(struct Data_Store_Queue *q)
{
    bool isfull = false;
    if(((q->tail - q->head) & QUEUE_SIZE_MASK(q)) == QUEUE_SIZE_MASK(q))
    {
        isfull = true;
#if DS_CFG_M_DEBUG_QUEUE
        DEBUG_LOG("queue is full");
#endif
    }else{
        isfull = false;
    }
    return isfull;
}

bool data_store_queue_is_empty(struct Data_Store_Queue *q) 
{
    bool isempty = false;
    if(q->head == q->tail)
    {
        isempty = true;
// #if DS_CFG_M_DEBUG_QUEUE // alway triggered, so comment it
//         DEBUG_LOG("queue is empty");
// #endif
    }else{
        isempty = false;
    }
  return isempty;
}


void data_store_enqueue(struct Data_Store_Queue *q, struct Data_Store_Message *msg)
{
    DATA_STORE_MUTEX_LOCK();
    for(int i = 0; i < msg->size; i++) 
    {
        /* Is buffer full?: move head */
        if(data_store_queue_is_full(q)) {
            /* Is going to overwrite the oldest byte */
            /* Increase tail index */
            q->head = ((q->head + 1) & QUEUE_SIZE_MASK(q));
        }

        /* Place data in buffer: always move tail */
        q->buf[q->tail] = msg->buf[i];
        q->tail = ((q->tail + 1) & QUEUE_SIZE_MASK(q));
    }
    DATA_STORE_MUTEX_UNLOCK();
}

int32_t data_store_dequeue(struct Data_Store_Queue *q, struct Data_Store_Message *msg)
{
    DATA_STORE_MUTEX_LOCK();
    if(data_store_queue_is_empty(q)) {
        /* No items */
        DATA_STORE_MUTEX_UNLOCK();
        return 0;
    }
    for(int i = 0; i < msg->size; i++)
    {
        msg->buf[i] = q->buf[q->head];
        q->head = ((q->head + 1) & QUEUE_SIZE_MASK(q));

        if(data_store_queue_is_empty(q)) {
            /* No items */
            DATA_STORE_MUTEX_UNLOCK();
            return i+1;
        }

    }
    DATA_STORE_MUTEX_UNLOCK();
    return  -1;         // never be here: avoid warning
}



