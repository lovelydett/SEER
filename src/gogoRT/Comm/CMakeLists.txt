cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
project(gogort_comm)

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} gogort_utils glog)

target_include_directories(${PROJECT_NAME} PUBLIC ./)