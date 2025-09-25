#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "data_store_config.h"
#include "data_store.h"


#define TEST_FILE_NAME "/home/hm/Desktop/data_store_module/test/flash.img"
FILE *g_flash_file;
pthread_mutex_t g_mux_lock = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char **argv)
{
    printf("===============================================================\n");
    printf("hi this is data store test program\n");
    printf("===============================================================\n");
    g_flash_file = fopen(TEST_FILE_NAME, "rb+");


    data_store_init();

    uint32_t num = data_store_get_history_num();
    printf("there are %d history record\n", num);
    for(uint32_t i = 1; i <= num; i++)
    {
        uint8_t buf[HISTORY_MSG_SIZE];
        data_store_get_record_history(i, buf);

        printf("export entry is %u, internal id:%u --> %s\n", i, *(uint32_t *)buf, buf+4);
    }


    return 0;
}


