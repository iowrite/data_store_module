# Build test executable and link with data_store static library
add_executable(data_store_test_write_history test_write_history.c port.c)
target_compile_options(data_store_test_write_history PRIVATE -std=gnu99)
target_link_libraries(data_store_test_write_history PRIVATE data_store_lib pthread)
target_include_directories(data_store_test_write_history PRIVATE ${PROJECT_SOURCE_DIR}/src)


add_executable(data_store_test_read_history test_read_history.c port.c)
target_link_libraries(data_store_test_read_history PRIVATE data_store_lib pthread)
target_include_directories(data_store_test_read_history PRIVATE ${PROJECT_SOURCE_DIR}/src)
