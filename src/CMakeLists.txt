
cmake_minimum_required(VERSION 3.10)
project(data_store_module)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use clang as the C compiler
set(CMAKE_C_COMPILER "clang")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF) # Use 'c99', not 'gnu99'
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(.)


# 自动收集所有.c和.h文件（排除old目录）
file(GLOB_RECURSE SOURCES
	"*.c"
	"*.h"
)
list(FILTER SOURCES EXCLUDE REGEX "old/")

# 创建静态库
add_library(data_store STATIC ${SOURCES})

# 编译选项
target_compile_options(data_store PRIVATE
	-std=c99
	-Wall
	-Wextra
	-Wconversion
	-Wstrict-prototypes
	-ffunction-sections
	-fdata-sections
	-O0
)