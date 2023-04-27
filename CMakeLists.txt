cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)

# Use GTest in CMake
add_subdirectory(third_party/googletest)
add_subdirectory(third_party/yaml-cpp)
add_subdirectory(third_party/glog)

# Yuting: add sub directories
add_subdirectory(src)
add_subdirectory(experimental)