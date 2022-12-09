cmake_minimum_required(VERSION 3.5)
project(gogort_utils)

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC ./)