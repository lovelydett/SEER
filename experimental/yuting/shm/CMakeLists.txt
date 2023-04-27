project(exp_yuting_shm)
add_executable(shared_memory main.cpp)
target_link_libraries(shared_memory pthread)