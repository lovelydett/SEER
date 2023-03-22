cmake_minimum_required(VERSION 3.5)
set(MODULE_NAME gogort_schedulers)

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_library(${MODULE_NAME} STATIC ${SRC_LIST})
target_link_libraries(${MODULE_NAME} PUBLIC glog gogort_risk yaml-cpp lib_utils)

target_include_directories(${MODULE_NAME} PUBLIC ./)