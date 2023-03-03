cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
project(Gogo)

FILE(GLOB SRC_LIST "*.cpp")

# Yuting: use yaml-cpp
include_directories(${YAML_CPP_INCLUDE_DIR}/include ${YAML_CPP_INCLUDE_DIR})
#find_package(yaml-cpp REQUIRED)
#include_directories(${YAML_CPP_INCLUDE_DIR})

# Yuting: use glog
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(glog REQUIRED libglog)
#find_package(glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIR}/include ${GLOG_INCLUDE_DIR})

# Use thread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(${PROJECT_NAME} ${SRC_LIST} ../experimental/zifeng/try_serial_comm/main.cc)

target_link_libraries(${PROJECT_NAME} gogort gogo_messages gogo_tasks glog yaml-cpp)

add_subdirectory(gogoRT)
add_subdirectory(messages)
add_subdirectory(tasks)