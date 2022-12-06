cmake_minimum_required(VERSION 3.5)
project(gogo)

set(CMAKE_CXX_STANDARD 20)

# Yuting: add dependency for google test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Yuting: add sub directories
add_subdirectory(src)
# add_subdirectory(experimental)