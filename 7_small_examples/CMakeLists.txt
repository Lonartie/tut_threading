set(PROJ_NAME 7_small_examples)

project(${PROJ_NAME})
add_executable(${PROJ_NAME} ${PROJ_NAME}.cpp)
target_compile_options(${PROJ_NAME} PRIVATE -fopenmp)
target_link_libraries(${PROJ_NAME} PRIVATE -fopenmp)
