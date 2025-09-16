
#include <stdint.h>
#include "data_store_config.h"
#include "data_store_queue.h"
#include "data_store_private.h"

struct Event_Record{
    struct Record_Context rc;
    /* member for event itself */
};


struct Event_Record g_event_record;



struct Queue_Event g_queue_event = {
    .queue.buf = g_queue_event.buf,
    .queue.head = 0,
    .queue.tail = 0,
    .queue.size = sizeof(g_queue_event.buf)
};

int8_t data_strore_event_init(void)
{
    g_event_record.rc.cr.content_block_start = CONTENT_START_ADDR_EVENT/FLASH_BLOCK_SIZE;
    g_event_record.rc.cr.content_block_end = (CONTENT_START_ADDR_EVENT + CONTENT_LEN_EVENT)/FLASH_BLOCK_SIZE;
    g_event_record.rc.cr.dir_bit_block_start = DIRECTORY_START_ADDR_FLAGS_EV/FLASH_BLOCK_SIZE;
    g_event_record.rc.cr.dir_flag_block_start = DIRECTORY_START_ADDR_FLAGS_EV/FLASH_BLOCK_SIZE;
    
    return 0;
}


void data_strore_event_task(void)
{






}
