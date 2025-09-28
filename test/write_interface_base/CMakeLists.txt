include(../data_store_test.cmake)

add_library(${DATA_STORE_LIB_NAME} OBJECT ${DATA_STORE_SOURCES} ${DATA_STORE_HEADERS})




add_library(${DATA_STORE_LIB_CONFIG_NAME} INTERFACE)
target_compile_definitions(${DATA_STORE_LIB_CONFIG_NAME} INTERFACE 
    DATA_STORE_USER_CONFIG_FILE="/home/hm/Desktop/data_store_module/test/${DIR_NAME}/data_store_user_config.h"
)

# 主库链接接口库
target_link_libraries(${DATA_STORE_LIB_NAME} PRIVATE data_store_interface ${DATA_STORE_LIB_CONFIG_NAME})


# Build test executable and link with data_store static library
add_executable(${TEST_EXE_NAME} test_write_history.c port.c)
target_compile_options(${TEST_EXE_NAME} PRIVATE -std=gnu99)
target_link_libraries(${TEST_EXE_NAME} PRIVATE ${DATA_STORE_LIB_NAME} pthread)
target_include_directories(${TEST_EXE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(${TEST_EXE_NAME} PRIVATE 
    DATA_STORE_USER_CONFIG_FILE="/home/hm/Desktop/data_store_module/test/${DIR_NAME}/data_store_user_config.h"
)


# 添加测试
enable_testing()
add_test(NAME ${TEST_EXE_NAME} COMMAND ${TEST_EXE_NAME} --integration-test)