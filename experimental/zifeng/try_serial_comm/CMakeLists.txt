add_executable(try_serial main.cc)
find_package(serial REQUIRED)
include_directories(${serial_INCLUDE_DIRS})
target_link_libraries(try_serial ${serial_LIBRARIES})

message("${serial_INCLUDE_DIRS}")