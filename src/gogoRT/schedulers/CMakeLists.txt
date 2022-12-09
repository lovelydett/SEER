cmake_minimum_required(VERSION 3.5)
project(gogort_schedulers)

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} PUBLIC glog)

target_include_directories(${PROJECT_NAME} PUBLIC ./)