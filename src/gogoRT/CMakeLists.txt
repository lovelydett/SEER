cmake_minimum_required(VERSION 3.5)
project(gogort)

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} gogort_utils gogort_schedulers gogort_comm glog)

target_include_directories(${PROJECT_NAME} PUBLIC ./)

add_subdirectory(utils)
add_subdirectory(Comm)
add_subdirectory(schedulers)