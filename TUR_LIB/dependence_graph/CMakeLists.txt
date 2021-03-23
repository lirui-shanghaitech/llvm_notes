cmake_minimum_required(VERSION 3.10) 

find_package(LLVM REQUIRED CONFIG)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++14") 
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../TUR_BUILD_LIB)

# include the print dir and current dir
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../tur_print/ ./)
include_directories(${LLVM_INCLUDE_DIRS}) 
add_definitions(${LLVM_DEFINITIONS}) 

aux_source_directory(. FINDFUNCTIONS_SRCS)
add_library(dependence_graph ${FINDFUNCTIONS_SRCS})