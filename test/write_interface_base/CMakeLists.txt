target_compile_definitions(data_store_interface INTERFACE 
    DATA_STORE_USER_CONFIG_FILE="/home/hm/Desktop/data_store_module/test/write_interface_base/data_store_user_config.h"
)



# Build test executable and link with data_store static library
add_executable(data_store_test_write_history test_write_history.c port.c)
target_compile_options(data_store_test_write_history PRIVATE -std=gnu99)
target_link_libraries(data_store_test_write_history PRIVATE data_store_lib pthread)
target_include_directories(data_store_test_write_history PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(data_store_test_write_history PRIVATE 
    DATA_STORE_USER_CONFIG_FILE="/home/hm/Desktop/data_store_module/test/write_interface_base/data_store_user_config.h"
)
