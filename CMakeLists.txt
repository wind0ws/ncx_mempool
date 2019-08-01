cmake_minimum_required(VERSION 3.14)
project(nxc_mempool C)

set(CMAKE_C_STANDARD 99)

#if (CMAKE_CURRENT_BINARY_DIR MATCHES "debug")
#    message(STATUS "Debug build detected")
#    add_compile_options("-DNCX_DEBUG") # <-- change this to have an effect on the build
#else()
#    message(STATUS "Release build detected")
#    add_compile_options("-DNCX_RELEASE") # <-- change this to have an effect on the build
#endif()

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
message( STATUS "Current Machine Architecture: ${ARCHITECTURE}" )
if( ${ARCHITECTURE} MATCHES  "64" )
    message(STATUS "detect 64bit machine. set NCX_PTR_SIZE=8")
    add_compile_options("-DNCX_PTR_SIZE=8")
else()
    add_compile_options("-DNCX_PTR_SIZE=4")
endif()

#ARCH := $(shell getconf LONG_BIT)
#ifeq ($(ARCH),64)
#CFLAGS+= -DNCX_PTR_SIZE=8
#else
#CFLAGS+= -DNCX_PTR_SIZE=4
#endif

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -Os -Wall -W -DPAGE_MERGE -DLOG_LEVEL=4")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIE -pie")

add_executable(nxc_mempool
        ncx_slab.c
        bench.c
        main.c
        )