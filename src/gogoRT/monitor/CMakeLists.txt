cmake_minimum_required(VERSION 3.5)
set(MODULE_NAME "lib_monitor")

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_library(${MODULE_NAME} ${SRC_LIST})

target_include_directories(${MODULE_NAME} PUBLIC ./)

target_link_libraries(${MODULE_NAME} lib_utils)