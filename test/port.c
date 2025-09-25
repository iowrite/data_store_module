#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "data_store_port.h"
#include "data_store_private.h"
#include "data_store_config.h"





extern FILE *g_flash_file;

extern pthread_mutex_t g_mux_lock;

int8_t data_store_port_read_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len)
{
    if(len > 0){
        uint32_t addr = block_index * FLASH_BLOCK_SIZE + page_index * FLASH_PAGE_SIZE + offset;
        fseek(g_flash_file, addr, SEEK_SET);
        fread(buf, 1, len, g_flash_file);
    }


    return 0;
}


int8_t data_store_port_write_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len)
{
    if(len > 0)
    {
        uint32_t addr = block_index * FLASH_BLOCK_SIZE + page_index * FLASH_PAGE_SIZE + offset;
        fseek(g_flash_file, addr, SEEK_SET);
        fwrite(buf, 1, len, g_flash_file);
        fflush(g_flash_file);
    }


    return 0;
}

int8_t data_store_port_erase_flash(uint32_t block_index, uint32_t len)
{
    if(len > 0)
    {
        uint32_t addr = block_index * FLASH_BLOCK_SIZE;
        fseek(g_flash_file, addr, SEEK_SET);
        uint8_t *buff = malloc(len * FLASH_BLOCK_SIZE);
        memset(buff, 0xff, len * FLASH_BLOCK_SIZE);
        fwrite(buff, 1, len * FLASH_BLOCK_SIZE, g_flash_file);
        fflush(g_flash_file);
        free(buff);
    }


    return 0;
}



uint32_t data_store_port_get_tick(void)
{




    return 0;

}

void data_store_port_mux_lock(void)
{
    pthread_mutex_lock(&g_mux_lock);
}

void data_store_port_mux_unlock(void)
{
    pthread_mutex_unlock(&g_mux_lock);
}


