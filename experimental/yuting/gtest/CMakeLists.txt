cmake_minimum_required(VERSION 3.5)
project(exp_yuting_gtest)

FILE(GLOB SRC_LIST "main.cpp")

enable_testing()

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} gtest GTest::gtest_main)

include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME})