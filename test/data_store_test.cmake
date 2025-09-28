get_filename_component(DIR_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set(DATA_STORE_LIB_NAME "data_store_static_lib_${DIR_NAME}")
set(DATA_STORE_LIB_CONFIG_NAME "data_store_interface_${DIR_NAME}")
set(TEST_EXE_NAME "data_store_test_${DIR_NAME}")