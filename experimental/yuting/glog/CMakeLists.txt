cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
project(exp_yuting_glog)

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_executable(${PROJECT_NAME} ${SRC_LIST} ../context/main.cpp)

# Yuting: dependency for glog
target_link_libraries(${PROJECT_NAME} PUBLIC glog)