# Build test executable and link with data_store static library
add_executable(data_store_test_write_history test_write_history.c port.c)
target_compile_options(data_store_test_write_history PRIVATE -std=gnu99)
target_link_libraries(data_store_test_write_history PRIVATE data_store_lib pthread)
target_include_directories(data_store_test_write_history PRIVATE ${PROJECT_SOURCE_DIR}/src)


add_executable(data_store_test_read_history test_read_history.c port.c)
target_link_libraries(data_store_test_read_history PRIVATE data_store_lib pthread)
target_include_directories(data_store_test_read_history PRIVATE ${PROJECT_SOURCE_DIR}/src)




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

