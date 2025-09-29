#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "data_store.h"
#include "data_store_config.h"


#define HISTORY_MSG_GEN_INTERVAL_MS   100

#define TEST_WRITE_TOTAL_COUNT (MAX_STORE_SPAN-1)

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
    while (i < TEST_WRITE_TOTAL_COUNT) {
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
    pthread_join(data_store_thread, NULL);

    uint32_t id[MAX_STORE_SPAN] = {0};
    struct Record_Date date[MAX_STORE_SPAN] = {0};
    data_store_get_history_summary(id, date);
    int ret = 0;
    for (int i = 0; i < TEST_WRITE_TOTAL_COUNT; i++) {
        printf("id: %d, date: %04d-%02d-%02d\n", id[i], date[i].year, date[i].month, date[i].day);

        if(i != id[i]-1)
        {
            ret = -1;                                  // test fail
        }


        if(i>0)
        {
            struct tm timeinfo = {0};
            timeinfo.tm_year = date[i].year - 1900;     // 年份从1900开始计算
            timeinfo.tm_mon = date[i].month-1;             // 月份：0-11 (0代表1月)
            timeinfo.tm_mday = date[i].day;            // 日期：1-31
            timeinfo.tm_hour = 0;            // 小时：0-23
            timeinfo.tm_min = 0;             // 分钟：0-59
            timeinfo.tm_sec = 0;             // 秒：0-59
            timeinfo.tm_isdst = -1;          // 夏令时：-1(自动判断), 0(否), 1(是)
            // 转换为Unix时间戳
            time_t timestamp = mktime(&timeinfo);

            struct tm timeinfo_last = {0};
            timeinfo_last.tm_year = date[i-1].year - 1900;     // 年份从1900开始计算
            timeinfo_last.tm_mon = date[i-1].month-1;             // 月份：0-11 (0代表1月)
            timeinfo_last.tm_mday = date[i-1].day;            // 日期：1-31
            timeinfo_last.tm_hour = 0;            // 小时：0-23
            timeinfo_last.tm_min = 0;             // 分钟：0-59
            timeinfo_last.tm_sec = 0;             // 秒：0-59
            timeinfo_last.tm_isdst = -1;          // 夏令时：-1(自动判断), 0(否), 1(是)
            // 转换为Unix时间戳
            time_t timestamp_last = mktime(&timeinfo_last);

            if(timestamp-timestamp_last != 86400*2)     // 2 days
            {
                ret = -2;                                  // test fail
            }
        }
    }


    return ret;                                           // test pass
}


