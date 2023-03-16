set(MODULE_NAME lib_mock_task)

FILE(GLOB SRC_LIST "*.cpp")

add_library(${MODULE_NAME} STATIC ${SRC_LIST})

target_link_libraries(${MODULE_NAME} gogort gogo_messages lib_mock_message glog yaml-cpp)

target_include_directories(${MODULE_NAME} PUBLIC ./)