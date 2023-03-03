add_executable(try_serial main.cpp)
find_package(serial REQUIRED)
include_directories(${serial_INCLUDE_DIRS})
target_link_libraries(try_comm ${serial_LIBRARIES})
