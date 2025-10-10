#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "data_store_config.h"
#include "data_store.h"

#define TEST_FILE_NAME "/home/hm/Desktop/data_store_module/test/read_interface_base/flash.img"
#define OUTPUT_FILE "program_output.txt"
FILE *g_flash_file;
pthread_mutex_t g_mux_lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
    // 重定向标准输出到文件
    FILE *output_file = freopen(OUTPUT_FILE, "w", stdout);
    if (output_file == NULL) {
        // 如果重定向失败，使用stderr输出错误信息
        fprintf(stderr, "Failed to redirect stdout to file\n");
        return -1;
    }

    
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

    // 刷新缓冲区确保所有内容都写入文件
    fflush(stdout);
    
    // 关闭文件（程序退出时会自动关闭，但显式关闭是好习惯）
    fclose(output_file);

    int ret = system("diff /home/hm/Desktop/data_store_module/test/read_interface_base/output.txt program_output.txt");

    if(ret == 0)
    {
        return 0;
    }else{
        return -1;
    }

}