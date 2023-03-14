cmake_minimum_required(VERSION 3.5)
set(MODULE_NAME gogort_risk_type)

FILE(GLOB SRC_LIST "*.cpp")

add_library(${MODULE_NAME} STATIC ${SRC_LIST})

target_link_libraries(${MODULE_NAME} gogort glog yaml-cpp)

target_include_directories(${MODULE_NAME} PUBLIC ./)