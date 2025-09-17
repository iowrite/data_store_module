#include <stdint.h>
#include "data_store_config.h"


#ifndef __DATA_STORE_PORT_H__
#define __DATA_STORE_PORT_H__


int8_t data_store_port_read_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len);

int8_t data_store_port_write_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len);

int8_t data_store_port_erase_flash(uint32_t block_index, uint32_t len);

uint32_t data_store_port_get_tick(void);

void data_store_port_mux_lock(void);
void data_store_port_mux_unlock(void);

#if DS_CFG_P_OS
    #define DATA_STORE_MUTEX_LOCK()        data_store_port_mux_lock()
    #define DATA_STORE_MUTEX_UNLOCK()      data_store_port_mux_unlock()
#else
    #define DATA_STORE_MUTEX_LOCK()
    #define DATA_STORE_MUTEX_UNLOCK()
#endif



#endif // __DATA_STORE_PORT_H__