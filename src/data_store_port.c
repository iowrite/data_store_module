#include <stdint.h>
#include "data_store_port.h"
#include "data_store_private.h"


__attribute__((weak)) int8_t data_store_port_read_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len)
{
    UNUSED(block_index);
    UNUSED(page_index);
    UNUSED(offset);
    UNUSED(buf);
    UNUSED(len);




    return 0;
}


__attribute__((weak)) int8_t data_store_port_write_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len)
{
    
    UNUSED(block_index);
    UNUSED(page_index);
    UNUSED(offset);
    UNUSED(buf);
    UNUSED(len);


    return 0;
}

__attribute__((weak)) int8_t data_store_port_erase_flash(uint32_t block_index, uint32_t len)
{
    
    UNUSED(block_index);
    UNUSED(len);


    return 0;
}



__attribute__((weak)) uint32_t data_store_port_get_tick(void)
{




    return 0;

}


__attribute__((weak)) int8_t data_store_port_get_date(uint16_t *year, uint8_t *month, uint8_t *day)
{
    UNUSED(year);
    UNUSED(month);
    UNUSED(day);
    return 0;
}


__attribute__((weak)) void data_store_port_mux_lock(void)
{



}

__attribute__((weak)) void data_store_port_mux_unlock(void)
{

}


