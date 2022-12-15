cmake_minimum_required(VERSION 3.5)
project(gogort_tests)

enable_testing()

# Pipe uni-test
add_executable(pipe_test Pipe_test.cpp)
target_link_libraries(pipe_test gtest GTest::gtest_main gogort_comm gogo_messages)


include(GoogleTest)
gtest_add_tests(TARGET pipe_test)