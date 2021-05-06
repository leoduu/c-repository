cmake_minimum_required(VERSION 2.8.12)

# project name 
project(std-c)

# include directory
include_directories(${PROJECT_SOURCE_DIR}/template)
include_directories(${PROJECT_SOURCE_DIR}/vector)

# add executable file
add_executable(vector-c   ${PROJECT_SOURCE_DIR}/vector/vector-c.c)
add_executable(template-c ${PROJECT_SOURCE_DIR}/template/template-c.c)
