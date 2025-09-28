

add_library(data_store_static_lib_write_interface_base OBJECT ${DATA_STORE_SOURCES} ${DATA_STORE_HEADERS})


add_library(data_store_interface_write INTERFACE)
target_compile_definitions(data_store_interface_write INTERFACE 
    DATA_STORE_USER_CONFIG_FILE="/home/hm/Desktop/data_store_module/test/write_interface_base/data_store_user_config.h"
)

# 主库链接接口库
target_link_libraries(data_store_static_lib_write_interface_base PRIVATE data_store_interface data_store_interface_write)



# Build test executable and link with data_store static library
add_executable(data_store_test_write_history test_write_history.c port.c)
target_compile_options(data_store_test_write_history PRIVATE -std=gnu99)
target_link_libraries(data_store_test_write_history PRIVATE data_store_static_lib_write_interface_base pthread)
target_include_directories(data_store_test_write_history PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(data_store_test_write_history PRIVATE 
    DATA_STORE_USER_CONFIG_FILE="/home/hm/Desktop/data_store_module/test/write_interface_base/data_store_user_config.h"
)


# 添加测试
enable_testing()
add_test(NAME data_store_test_write_history COMMAND data_store_test_write_history --integration-test)