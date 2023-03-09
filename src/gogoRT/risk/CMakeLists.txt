cmake_minimum_required(VERSION 3.5)
set(MODULE_NAME gogort_risk)

FILE(GLOB SRC_LIST "*.cpp" "*.h")

add_library(${MODULE_NAME} STATIC ${SRC_LIST})
target_link_libraries(${MODULE_NAME} PUBLIC glog)
target_include_directories(${MODULE_NAME} PUBLIC ./)

add_subdirectory(handler)
add_subdirectory(risk_type)