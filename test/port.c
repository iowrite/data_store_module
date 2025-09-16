#include <stdint.h>
#include "data_store_port.h"
#include "data_store_private.h"
#include "data_store_config.h"






int8_t data_store_port_read_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len)
{
    UNUSED(block_index);
    UNUSED(page_index);
    UNUSED(offset);
    UNUSED(buf);
    UNUSED(len);
    




    return 0;
}


int8_t data_store_port_write_flash(uint32_t block_index, uint32_t page_index, uint32_t offset, uint8_t* buf, uint32_t len)
{
    
    UNUSED(block_index);
    UNUSED(page_index);
    UNUSED(offset);
    UNUSED(buf);
    UNUSED(len);


    return 0;
}

int8_t data_store_port_erase_flash(uint32_t block_index, uint32_t len)
{
    
    UNUSED(block_index);
    UNUSED(len);


    return 0;
}



uint32_t data_store_port_get_tick(void)
{




    return 0;

}
