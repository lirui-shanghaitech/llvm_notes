cmake_minimum_required(VERSION 3.10) 

find_package(LLVM REQUIRED CONFIG)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++14") 

# include the print dir and current dir
include_directories(./../tur_print/ ./)
include_directories(${LLVM_INCLUDE_DIRS}) 
add_definitions(${LLVM_DEFINITIONS}) 

aux_source_directory(. FINDFUNCTIONS_SRCS)
add_library(find_functions ${FINDFUNCTIONS_SRCS})