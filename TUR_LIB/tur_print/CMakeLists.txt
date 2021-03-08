
cmake_minimum_required(VERSION 3.10) 

find_package(LLVM REQUIRED CONFIG) 
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}") 
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}") 
# Needed to use support library 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++14") 

include_directories(${LLVM_INCLUDE_DIRS}) 
add_definitions(${LLVM_DEFINITIONS}) 

aux_source_directory(. DIR_HI_PRINT_SRCS)
add_library(tur_print ${DIR_HI_PRINT_SRCS})


