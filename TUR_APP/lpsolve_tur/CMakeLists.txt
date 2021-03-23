project(lpsolve_tur) 
cmake_minimum_required(VERSION 3.10) 

set(Outputname "lpsolve_tur")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++14 -g") 


include_directories(../../LP_LIB/include/) 
link_directories(../../LP_LIB/lib/)

aux_source_directory(. SRCS)
add_executable(${Outputname} ${SRCS}) 
target_link_libraries(${Outputname} lpsolve55)


add_test(Test_${Outputname} ${Outputname})
enable_testing()