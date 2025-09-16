#include <stdint.h>



#ifndef __DATA_STORE_PORT_H__
#define __DATA_STORE_PORT_H__


int8_t data_store_port_read_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len);

int8_t data_store_port_write_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len);

int8_t data_store_port_erase_flash(uint32_t block_index, uint32_t len);

uint32_t data_store_port_get_tick(void);



#endif // __DATA_STORE_PORT_H__