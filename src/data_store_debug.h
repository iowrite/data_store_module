#include "data_store_config.h"



#ifndef  DATA_STORE_DEBUG_H
#define  DATA_STORE_DEBUG_H







#if DS_CFG_M_DEBUG && DS_CFG_P_FULL_STD_CLIB
    #include <stdio.h>
    #define DEBUG_LOG(fmt, ...) \
        printf("[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DEBUG_LOG(fmt, ...)
#endif




#endif          




