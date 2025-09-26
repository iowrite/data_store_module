#include <stdint.h>
#include "data_store_config.h"


#ifndef __DATA_STORE_PORT_H__
#define __DATA_STORE_PORT_H__


int8_t data_store_port_read_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len);

int8_t data_store_port_write_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len);

int8_t data_store_port_erase_flash(uint32_t block_index, uint32_t len);

uint32_t data_store_port_get_tick(void);
int8_t data_store_port_get_date(uint16_t *year, uint8_t *month, uint8_t *day);

void data_store_port_mux_lock(void);
void data_store_port_mux_unlock(void);




#endif // __DATA_STORE_PORT_H__