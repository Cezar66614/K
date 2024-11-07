cmake_minimum_required(VERSION 3.0.0)
project(K VERSION 0.1.0)

set(
  PROG_SRCS

  src/main.c
)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} ${PROG_SRCS})
