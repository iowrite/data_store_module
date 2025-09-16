#include <stdbool.h>
#include "data_store_queue.h"
#include "data_store_private.h"





void data_store_enqueue(struct Data_Store_Queue *q, struct Data_Store_Message *msg)
{
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
}

int32_t data_store_dequeue(struct Data_Store_Queue *q, struct Data_Store_Message *msg)
{

    if(data_store_queue_is_empty(q)) {
        /* No items */
        return 0;
    }
    for(int i = MESSAGE_HEADER_SIZE; i < msg->size; i++)
    {
        msg->buf[i] = q->buf[q->head];
        q->head = ((q->head + 1) & QUEUE_SIZE_MASK(q));

        if(data_store_queue_is_empty(q)) {
            /* No items */
            return i+1;
        }

    }
    return  -1;         // never be here: avoid warning

}



