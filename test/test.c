
#include <stdio.h>


#define TEST_FILE_NAME "flash.img"
FILE *g_flash_file;


int main(int argc, char **argv)
{ 
    printf("hi this is data store test program\n");
    g_flash_file = fopen(TEST_FILE_NAME, "wb+");



    



}


