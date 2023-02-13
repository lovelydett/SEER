cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
project(gogo_proj)

# Use GTest in CMake
add_subdirectory(third_party/googletest)

# Use yaml-cpp in CMake
add_subdirectory(third_party/yaml-cpp)

# Use glogging in CMake
add_subdirectory(third_party/glog)

# Yuting: add sub directories
add_subdirectory(src)
# add_subdirectory(experimental)