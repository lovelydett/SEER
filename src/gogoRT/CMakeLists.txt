cmake_minimum_required(VERSION 3.5)
set(MODULE_NAME gogort)

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_library(${MODULE_NAME} STATIC ${SRC_LIST})
target_link_libraries(${MODULE_NAME} gogort_utils gogort_schedulers gogort_comm glog yaml-cpp lib_mock_task)

target_include_directories(${MODULE_NAME} PUBLIC ./)

add_subdirectory(utils)
add_subdirectory(Comm)
add_subdirectory(schedulers)
add_subdirectory(unit_tests)
add_subdirectory(risk)