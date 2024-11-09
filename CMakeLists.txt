cmake_minimum_required(VERSION 3.5.0)
project(K VERSION 0.1.0)

set(
  PROG_SRCS

  src/main.c
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} ${PROG_SRCS})

target_link_libraries(${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function)
