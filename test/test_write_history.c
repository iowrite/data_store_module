#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "data_store.h"


#define HISTORY_MSG_GEN_INTERVAL_MS   100

#define TEST_FILE_NAME "/home/hm/Desktop/data_store_module/test/flash.img"
FILE *g_flash_file;
pthread_mutex_t g_mux_lock = PTHREAD_MUTEX_INITIALIZER;
void *data_store_thread_func(void *arg) {
    printf("[thread 1]-->init\n");
    data_store_init();
    while (1) {
        data_store_task();
    }
    return NULL;
}


void *generate_history_thread_func(void *arg) {
    printf("[thread 2]-->init\n");
    int i = 0;
    while (1) {
        time_t now = time(NULL); // 获取当前时间戳
        struct tm *local = localtime(&now); // 转换为本地时间结构
        uint8_t msg[252];
        memset(msg, ' ', sizeof msg);
        msg[250] = '\n';
        msg[251] = '\0';
        sprintf((char *)msg, "time: %04d-%02d-%02d %02d:%02d:%02d, i: %d",
                local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
                local->tm_hour, local->tm_min, local->tm_sec, i);
        printf("====> +++++\n");
        data_store_post_a_history(msg, sizeof msg);
        printf("[thread 1]-->generate a history: %s\n", msg);
        usleep(HISTORY_MSG_GEN_INTERVAL_MS*1000);
        i++;
        
        
    }
    return NULL;
}


int main(int argc, char **argv)
{
    printf("===============================================================\n");
    printf("hi this is data store test program\n");
    printf("===============================================================\n");
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

    // Optionally, do other work here or just wait for the thread
    pthread_join(data_store_thread, NULL);
    pthread_join(generate_history_thread, NULL);

    return 0;
}


