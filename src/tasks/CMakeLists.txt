cmake_minimum_required(VERSION 3.5)
project(gogo_tasks)

FILE(GLOB SRC_LIST "*.cpp")

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} gogort gogo_messages glog)

target_include_directories(${PROJECT_NAME} PUBLIC ./)