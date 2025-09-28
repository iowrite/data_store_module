#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "data_store.h"
#include "data_store_config.h"


#define HISTORY_MSG_GEN_INTERVAL_MS   10

#define TEST_FILE_NAME "flash.img"
FILE *g_flash_file;
pthread_mutex_t g_mux_lock = PTHREAD_MUTEX_INITIALIZER;
volatile int g_exit_flag = 0;
void *data_store_thread_func(void *arg) {
    printf("[thread 1]-->init\n");
    data_store_init();
    while (!g_exit_flag) {
        data_store_task();
    }
    printf("[thread 1]--> exit\n");
    return NULL;
}


void *generate_history_thread_func(void *arg) {
    printf("[thread 2]-->init\n");
    int i = 0;
    while (i <= MAX_HISTROY_RECORD_NUM) {
        time_t now = time(NULL); // 获取当前时间戳
        struct tm *local = localtime(&now); // 转换为本地时间结构
        uint8_t msg[HISTORY_MSG_SIZE-MESSAGE_HEADER_SIZE];
        memset(msg, ' ', sizeof msg);
        msg[HISTORY_MSG_SIZE-MESSAGE_HEADER_SIZE-1] = '\n';
        msg[HISTORY_MSG_SIZE-MESSAGE_HEADER_SIZE-1] = '\0';
        sprintf((char *)msg, "time: %04d-%02d-%02d %02d:%02d:%02d, i: %d",
                local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
                local->tm_hour, local->tm_min, local->tm_sec, i);
        printf("====> +++++\n");
        data_store_post_a_history(msg, sizeof msg);
        printf("[thread 2]-->generate a history: %s\n", msg);
        usleep(HISTORY_MSG_GEN_INTERVAL_MS*1000);
        i++;
        // ...existing code...
    }

    g_exit_flag = 1; // 通知 data_store_thread_func 退出
    printf("[thread 2]--> exit\n");
    return NULL;
}



// void *export_func(void *arg) {
//     printf("[thread 2]-->init\n");


//     data_store_batch_export_history_start();

//     uint32_t num = data_store_get_history_num();
//     printf("there are %d history record\n", num);
//     for(uint32_t i = 1; i <= num; i++)
//     {
//         uint8_t buf[HISTORY_MSG_SIZE];
//         data_store_get_record_history(i, buf);

//         printf("export entry is %u, internal id:%u --> %s\n", i, *(uint32_t *)buf, buf+4);

//     }

//     data_store_batch_export_history_end();


//     return NULL;
// }

int main(int argc, char **argv)
{

    printf("===============================================================\n");
    printf("hi this is data store test program\n");
    printf("test app: using config file: %s\n", DATA_STORE_USER_CONFIG_FILE);
    printf("===============================================================\n");

    system("dd if=/dev/zero bs=1M count=64 | tr '\\000' '\\377' > flash.img");

    g_flash_file = fopen(TEST_FILE_NAME, "rb+");


    pthread_t data_store_thread;
    if (pthread_create(&data_store_thread, NULL, data_store_thread_func, NULL) != 0) {
        perror("Failed to create data_store_thread");
        return -1;
    }

    pthread_t generate_history_thread;
    if (pthread_create(&generate_history_thread, NULL, generate_history_thread_func, NULL) != 0) {
        perror("Failed to create generate_history_thread");
        return -1;
    }
    pthread_join(generate_history_thread, NULL);

    // pthread_t export_thread;
    // if (pthread_create(&export_thread, NULL, data_store_thread_func, NULL) != 0) {
    //     perror("Failed to create data_store_thread");
    //     return -1;
    // }

    // Optionally, do other work here or just wait for the thread
    pthread_join(data_store_thread, NULL);


    return 0;
}


