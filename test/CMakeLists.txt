# 创建接口库来管理编译定义
add_library(data_store_interface_read INTERFACE)

target_compile_definitions(data_store_interface_read INTERFACE 
    DATA_STORE_USER_CONFIG_FILE="/home/hm/Desktop/data_store_module/test/read_interface_base/data_store_user_config.h"
)

# 主库链接接口库
target_link_libraries(data_store_lib PRIVATE data_store_interface_read)


add_subdirectory(read_interface_base)
add_subdirectory(write_interface_base)
