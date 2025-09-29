#warning "<this file is generate by default_config_to_user_config.sh>, your are useing default config, please define DATA_STORE_USER_CONFIG_FILE to your own config file,or delete this warning, edit this file accord your project"

#ifndef DATA_STORE_USER_CONFIG_FILE
#define DATA_STORE_USER_CONFIG_FILE "data_store_user_config.h"
#endif
#include DATA_STORE_USER_CONFIG_FILE

#ifndef DATA_STORE_CONFIG_H
#define DATA_STORE_CONFIG_H

// script process start 

/******************************************************************************
 * Configuration for store module
 ******************************************************************************/
    #define DS_CFG_M_DEBUG              0
    #define DS_CFG_M_DEBUG_QUEUE        0

/******************************************************************************
 * Configuration for store port
 ******************************************************************************/
    #define DS_CFG_P_FULL_STD_CLIB      1
    #define DS_CFG_P_OS                 1

/******************************************************************************
 * Configuration for store hardware
 ******************************************************************************/
    #define FLASH_TOTAL_SIZE        (uint32_t)(32*1024*1024)                // unit: byte
    #define FLASH_BLOCK_SIZE        4096                                    // unit: byte
    #define FLASH_PAGE_SIZE         256                                     // unit: byte

    #define BLOCKS_PER_FLASH       (FLASH_TOTAL_SIZE/FLASH_BLOCK_SIZE)
    #define PAGES_PER_BLOCK        (FLASH_BLOCK_SIZE/FLASH_PAGE_SIZE)

/******************************************************************************
 * Configuration for data storage regions
 * note: 
 ******************************************************************************/
// directory area
    #define DIRECTORY_START_ADDR            0x00000000
    #define DIRECTORY_LEN                   (uint32_t)(40*1024)            // byte

    #define DIRECTORY_START_ADDR_FLAGS_HIS      0x00000000
    #define DIRECTORY_LEN_FLAGS_HIS             (uint32_t)FLASH_BLOCK_SIZE      // one block
    #define DIRECTORY_START_ADDR_HISTORY        0x00001000
    #define DIRECTORY_LEN_BIT_HIS               (uint32_t)(16*1024)             // byte

    #define DIRECTORY_START_ADDR_FLAGS_EV       0x00005000
    #define DIRECTORY_LEN_FLAGS_EV              (uint32_t)FLASH_BLOCK_SIZE      // one block
    #define DIRECTORY_START_ADDR_EVENT          0x00006000
    #define DIRECTORY_LEN_BIT_EVENT             (uint32_t)(8*1024)              // byte

    #define DIRECTORY_START_ADDR_FLAGS_WA       0x00008000
    #define DIRECTORY_LEN_FLAGS_WA              (uint32_t)FLASH_BLOCK_SIZE      // one block
    #define DIRECTORY_START_ADDR_WAVE           0x00009000
    #define DIRECTORY_LEN_BIT_WAVE              (uint32_t)(4*1024)              // byte

// content area
    #define CONTENT_START_ADDR              0x0000b000
    #define CONTENT_LEN                     (uint32_t)(32*1024*1024)    // byte

    #define CONTENT_START_ADDR_HISTORY      0x0000b000
    #define CONTENT_LEN_HISTORY             (uint32_t)(16*1024*1024)    // byte
    #define CONTENT_START_ADDR_EVENT        0x0100b000
    #define CONTENT_LEN_EVENT               (uint32_t)(8*1024*1024)     // byte
    #define CONTENT_START_ADDR_WAVE         0x0180b000
    #define CONTENT_LEN_WAVE                (uint32_t)(8*1024*1024)

// store limit
    #define MAX_STORE_SPAN            30  // days
    #define MAX_HISTROY_RECORD_NUM    1000
    #define MAX_EVENT_RECORD_NUM      500
    #define MAX_WAVE_RECORD_NUM       100

// message size
// so, the user message size limit is: ** HISTORY_MSG_SIZE - MESSAGE_HEADER_SIZE ** 
    #define MESSAGE_HEADER_SIZE        8              // internal use, byte
    #define HISTORY_MSG_SIZE           256          // byte, must be multiple of FLASH_PAGE_SIZE
    #define EVENT_MSG_SIZE             256          // byte, must be multiple of FLASH_PAGE_SIZE
    #define WAVE_MSG_SIZE              (32*64)      // byte, must be multiple of FLASH_PAGE_SIZE




/******************************************************************************
 * Configuration for store strategy
 ******************************************************************************/






/******************************************************************************
 * Configuration for queue
 ******************************************************************************/
    #define DS_CFG_Q_HISTORY_SIZE       (HISTORY_MSG_SIZE*2)        // byte, must be power of 2
    #define DS_CFG_Q_EVENT_SIZE         (EVENT_MSG_SIZE*2)        // byte
    #define DS_CFG_Q_WAVE_SIZE          (2048*2)      // byte


// script process end

#endif