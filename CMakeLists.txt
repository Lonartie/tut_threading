cmake_minimum_required(VERSION 3.28)
project(threading)

set(CMAKE_CXX_STANDARD 20)

include_directories(./classes)

add_subdirectory(1_bank_sequential)
add_subdirectory(2_intro_threads)
add_subdirectory(3_bank_parallel)
add_subdirectory(4_bank_parallel_optimized)
add_subdirectory(5_graying_sequential)
add_subdirectory(6_graying_parallel)
