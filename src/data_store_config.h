
#ifndef DATA_STORE_USER_CONFIG_FILE
#define DATA_STORE_USER_CONFIG_FILE "data_store_user_config.h"
#endif
#include DATA_STORE_USER_CONFIG_FILE

#ifndef DATA_STORE_CONFIG_H
#define DATA_STORE_CONFIG_H

/******************************************************************************
 * Configuration for store module
 ******************************************************************************/
#ifndef DS_CFG_M_DEBUG
    #define DS_CFG_M_DEBUG              0
#endif
#ifndef DS_CFG_M_DEBUG_LOG
    #define DS_CFG_M_DEBUG_QUEUE        0
#endif

/******************************************************************************
 * Configuration for store port
 ******************************************************************************/
#ifndef DS_CFG_P_FULL_STD_CLIB
    #define DS_CFG_P_FULL_STD_CLIB      1
#endif
#ifndef DS_CFG_P_OS
    #define DS_CFG_P_OS                 1
#endif

/******************************************************************************
 * Configuration for store hardware
 ******************************************************************************/
#ifndef FLASH_TOTAL_SIZE
    #define FLASH_TOTAL_SIZE        (uint32_t)(32*1024*1024)                // unit: byte
#endif
#ifndef FLASH_BLOCK_SIZE
    #define FLASH_BLOCK_SIZE        4096                                    // unit: byte
#endif
#ifndef FLASH_PAGE_SIZE
    #define FLASH_PAGE_SIZE         256                                     // unit: byte
#endif

#ifndef BLOCKS_PER_FLASH
    #define BLOCKS_PER_FLASH       (FLASH_TOTAL_SIZE/FLASH_BLOCK_SIZE)
#endif
#ifndef PAGES_PER_BLOCK
    #define PAGES_PER_BLOCK        (FLASH_BLOCK_SIZE/FLASH_PAGE_SIZE)
#endif

/******************************************************************************
 * Configuration for data storage regions
 * note: 
 ******************************************************************************/
// directory area
#ifndef DIRECTORY_START_ADDR
    #define DIRECTORY_START_ADDR            0x00000000
#endif
#ifndef DIRECTORY_LEN
    #define DIRECTORY_LEN                   (uint32_t)(40*1024)            // byte
#endif

#ifndef DIRECTORY_START_ADDR_FLAGS_HIS
    #define DIRECTORY_START_ADDR_FLAGS_HIS      0x00000000
#endif
#ifndef DIRECTORY_LEN_FLAGS_HIS
    #define DIRECTORY_LEN_FLAGS_HIS             (uint32_t)FLASH_BLOCK_SIZE      // one block
#endif
#ifndef DIRECTORY_START_ADDR_HISTORY
    #define DIRECTORY_START_ADDR_HISTORY        0x00001000
#endif
#ifndef DIRECTORY_LEN_BIT_HIS
    #define DIRECTORY_LEN_BIT_HIS               (uint32_t)(16*1024)             // byte
#endif

#ifndef DIRECTORY_START_ADDR_FLAGS_EV
    #define DIRECTORY_START_ADDR_FLAGS_EV       0x00005000
#endif
#ifndef DIRECTORY_LEN_FLAGS_EV
    #define DIRECTORY_LEN_FLAGS_EV              (uint32_t)FLASH_BLOCK_SIZE      // one block
#endif
#ifndef DIRECTORY_START_ADDR_EVENT
    #define DIRECTORY_START_ADDR_EVENT          0x00006000
#endif
#ifndef DIRECTORY_LEN_BIT_EVENT
    #define DIRECTORY_LEN_BIT_EVENT             (uint32_t)(8*1024)              // byte
#endif

#ifndef DIRECTORY_START_ADDR_FLAGS_WA
    #define DIRECTORY_START_ADDR_FLAGS_WA       0x00008000
#endif
#ifndef DIRECTORY_LEN_FLAGS_WA
    #define DIRECTORY_LEN_FLAGS_WA              (uint32_t)FLASH_BLOCK_SIZE      // one block
#endif
#ifndef DIRECTORY_START_ADDR_WAVE
    #define DIRECTORY_START_ADDR_WAVE           0x00009000
#endif
#ifndef DIRECTORY_LEN_BIT_WAVE
    #define DIRECTORY_LEN_BIT_WAVE              (uint32_t)(4*1024)              // byte
#endif

// content area
#ifndef CONTENT_START_ADDR
    #define CONTENT_START_ADDR              0x0000b000
#endif
#ifndef CONTENT_LEN
    #define CONTENT_LEN                     (uint32_t)(32*1024*1024)    // byte
#endif

#ifndef CONTENT_START_ADDR_HISTORY
    #define CONTENT_START_ADDR_HISTORY      0x0000b000
#endif
#ifndef CONTENT_LEN_HISTORY
    #define CONTENT_LEN_HISTORY             (uint32_t)(16*1024*1024)    // byte
#endif
#ifndef CONTENT_START_ADDR_EVENT
    #define CONTENT_START_ADDR_EVENT        0x0100b000
#endif
#ifndef CONTENT_LEN_EVENT
    #define CONTENT_LEN_EVENT               (uint32_t)(8*1024*1024)     // byte
#endif
#ifndef CONTENT_START_ADDR_WAVE
    #define CONTENT_START_ADDR_WAVE         0x0180b000
#endif
#ifndef CONTENT_LEN_WAVE
    #define CONTENT_LEN_WAVE                (uint32_t)(8*1024*1024)
#endif

// store limit
#ifndef MAX_STORE_SPAN
    #define MAX_STORE_SPAN            30  // days
#endif
#ifndef MAX_HISTROY_RECORD_NUM
    #define MAX_HISTROY_RECORD_NUM    1000
#endif
#ifndef MAX_EVENT_RECORD_NUM
    #define MAX_EVENT_RECORD_NUM      500
#endif
#ifndef MAX_WAVE_RECORD_NUM
    #define MAX_WAVE_RECORD_NUM       100
#endif

// message size
// so, the user message size limit is: ** HISTORY_MSG_SIZE - MESSAGE_HEADER_SIZE ** 
#ifndef MESSAGE_HEADER_SIZE
    #define MESSAGE_HEADER_SIZE        8              // internal use, byte
#endif
#ifndef HISTORY_MSG_SIZE
    #define HISTORY_MSG_SIZE           256          // byte, must be multiple of FLASH_PAGE_SIZE
#endif
#ifndef EVENT_MSG_SIZE
    #define EVENT_MSG_SIZE             256          // byte, must be multiple of FLASH_PAGE_SIZE
#endif
#ifndef WAVE_MSG_SIZE
    #define WAVE_MSG_SIZE              (32*64)      // byte, must be multiple of FLASH_PAGE_SIZE
#endif




/******************************************************************************
 * Configuration for store strategy
 ******************************************************************************/






/******************************************************************************
 * Configuration for queue
 ******************************************************************************/
#ifndef DS_CFG_Q_HISTORY_SIZE
    #define DS_CFG_Q_HISTORY_SIZE       (HISTORY_MSG_SIZE*2)        // byte, must be power of 2
#endif
#ifndef DS_CFG_Q_EVENT_SIZE
    #define DS_CFG_Q_EVENT_SIZE         (EVENT_MSG_SIZE*2)        // byte
#endif
#ifndef DS_CFG_Q_WAVE_SIZE
    #define DS_CFG_Q_WAVE_SIZE          (2048*2)      // byte
#endif

#endif