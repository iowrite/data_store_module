

# Collect all .c files in src (excluding old/)
file(GLOB_RECURSE DATA_STORE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)
file(GLOB_RECURSE DATA_STORE_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
list(FILTER DATA_STORE_SOURCES EXCLUDE REGEX "/old/")
list(FILTER DATA_STORE_HEADERS EXCLUDE REGEX "/old/")

add_library(data_store_lib STATIC ${DATA_STORE_SOURCES} ${DATA_STORE_HEADERS})
target_include_directories(data_store_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(data_store_lib PRIVATE
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
