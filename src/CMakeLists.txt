cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
project(Gogo)

# Yuting: add dependency for glog
include(FetchContent)
FetchContent_Declare(
        glog
        URL https://github.com/google/glog/archive/refs/tags/v0.6.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(glog_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glog)

FILE(GLOB SRC_LIST "*.cpp")

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} gogort messages tasks glog)

add_subdirectory(gogoRT)
add_subdirectory(messages)
add_subdirectory(tasks)