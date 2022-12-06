cmake_minimum_required(VERSION 3.5)
project(gogort_comm)

FILE(GLOB SRC_LIST "*.cpp")

add_library(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} gogort_utils gogort)

target_include_directories(${PROJECT_NAME} PUBLIC ./)