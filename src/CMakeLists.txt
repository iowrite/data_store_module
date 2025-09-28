

# Collect all .c files in src (excluding old/)
file(GLOB_RECURSE DATA_STORE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)
file(GLOB_RECURSE DATA_STORE_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
list(FILTER DATA_STORE_SOURCES EXCLUDE REGEX "/old/")
list(FILTER DATA_STORE_HEADERS EXCLUDE REGEX "/old/")


set(DATA_STORE_SOURCES ${DATA_STORE_SOURCES} CACHE INTERNAL "Data store source files")
set(DATA_STORE_HEADERS ${DATA_STORE_HEADERS} CACHE INTERNAL "Data store header files")

# add_library(data_store_lib STATIC ${DATA_STORE_SOURCES} ${DATA_STORE_HEADERS})





# 创建接口库来管理编译定义
add_library(data_store_interface INTERFACE)

target_compile_options(data_store_interface INTERFACE
	-std=c99
	-Wall
	-Wextra
	-Wconversion
	-Wstrict-prototypes
	-ffunction-sections
	-fdata-sections
	-O0
	# -g
	# -fprofile-instr-generate 
	# -fcoverage-mapping
)
target_include_directories(data_store_interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


