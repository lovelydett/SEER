cmake_minimum_required(VERSION 3.5)
project(exp_yuting_gtest)

FILE(GLOB SRC_LIST "main.cpp")

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} gtest)