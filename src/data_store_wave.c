#include <stdint.h>
#include "data_store_queue.h"
#include "data_store_private.h"




struct Wave_Record{
    struct Record_Context rc;
    /* member for wave itself */

};



struct Message_Wave
{
    struct Data_Store_Message header;
    uint8_t buf[WAVE_MSG_SIZE];
};




struct Queue_Wave g_queue_wave = {
    .queue.buf = g_queue_wave.buf,
    .queue.head = 0,
    .queue.tail = 0,
    .queue.size = sizeof(g_queue_wave.buf)
};


int8_t data_strore_wave_init(void)
{
    

    return 0;

}

void data_strore_wave_task(void)
{






}



