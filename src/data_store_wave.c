#include <stdint.h>
#include "data_store_queue.h"
#include "data_store_private.h"




struct Wave_Record{
    struct Record_Context rc;
    /* member for wave itself */

};
struct Wave_Record g_wave_record;


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
    g_wave_record.rc.cr.content_block_start = CONTENT_START_ADDR_WAVE/FLASH_BLOCK_SIZE;
    g_wave_record.rc.cr.content_block_end = (CONTENT_START_ADDR_WAVE + CONTENT_LEN_WAVE)/FLASH_BLOCK_SIZE;
    g_wave_record.rc.cr.dir_bit_block_start = DIRECTORY_START_ADDR_FLAGS_WA/FLASH_BLOCK_SIZE;
    g_wave_record.rc.cr.dir_flag_block_start = DIRECTORY_START_ADDR_FLAGS_WA/FLASH_BLOCK_SIZE;
    g_wave_record.rc.cr.dir_bit_block_len = DIRECTORY_LEN_BIT_WAVE/FLASH_BLOCK_SIZE;

    return 0;

}

void data_strore_wave_task(void)
{






}



