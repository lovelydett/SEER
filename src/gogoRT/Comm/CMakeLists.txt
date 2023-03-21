cmake_minimum_required(VERSION 3.5)

SET(MODULE_NAME gogort_comm)

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_library(${MODULE_NAME} STATIC ${SRC_LIST})
target_link_libraries(${MODULE_NAME} lib_utils glog)

target_include_directories(${MODULE_NAME} PUBLIC ./)