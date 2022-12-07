cmake_minimum_required(VERSION 3.5)
project(gogort)

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_library(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} gogort_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ./)

# add_subdirectory(utils)
add_subdirectory(Comm)