set(MODULE_NAME lib_pyxis_rt)

FILE(GLOB SRC_LIST "*.cpp")

add_library(${MODULE_NAME} STATIC ${SRC_LIST})

target_link_libraries(${MODULE_NAME} glog yaml-cpp)

target_include_directories(${MODULE_NAME} PUBLIC ./)