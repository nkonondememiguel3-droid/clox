cmake_minimum_required(VERSION 3.10)
project(clox)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} 
  main.c
  memory.c memory.h
  common.c common.h
  chunk.c chunk.h
  debug.h debug.c
  value.c value.h
  vm.h vm.c
  compiler.c compiler.h
  scanner.h scanner.c
)

