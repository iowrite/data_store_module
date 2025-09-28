

add_library(data_store_static_lib_read_interface_base OBJECT ${DATA_STORE_SOURCES} ${DATA_STORE_HEADERS})


# 创建接口库来管理编译定义
add_library(data_store_interface_read INTERFACE)

target_compile_definitions(data_store_interface_read INTERFACE 
    DATA_STORE_USER_CONFIG_FILE="/home/hm/Desktop/data_store_module/test/read_interface_base/data_store_user_config.h"
)

# 主库链接接口库
target_link_libraries(data_store_static_lib_read_interface_base PRIVATE data_store_interface data_store_interface_read)



add_executable(data_store_test_read_history test_read_history.c port.c)
target_link_libraries(data_store_test_read_history PRIVATE data_store_static_lib_read_interface_base pthread)
target_include_directories(data_store_test_read_history PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(data_store_test_read_history PRIVATE 
    DATA_STORE_USER_CONFIG_FILE="/home/hm/Desktop/data_store_module/test/read_interface_base/data_store_user_config.h"
)




# target_compile_options(data_store_test_read_history PRIVATE
#     -g
#     -O0
# 	-fprofile-instr-generate 
# 	-fcoverage-mapping
# )
# # 链接时也必须加！否则不会链接 profiling runtime
# target_link_options(data_store_test_read_history PRIVATE -fprofile-instr-generate)


# 添加测试
enable_testing()
add_test(NAME data_store_test_read_history COMMAND data_store_test_read_history --integration-test)

