cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
project(Gogo)

FILE(GLOB SRC_LIST "*.cpp")

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} gogort messages tasks)

add_subdirectory(gogoRT)
add_subdirectory(messages)
add_subdirectory(tasks)