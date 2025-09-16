# Build test executable and link with data_store static library
add_executable(data_store_test test.c port.c)
target_link_libraries(data_store_test PRIVATE data_store_lib)
target_include_directories(data_store_test PRIVATE ${PROJECT_SOURCE_DIR}/src)
