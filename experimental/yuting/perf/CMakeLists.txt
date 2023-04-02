project(exp_yuting_perf)

add_executable(try_perf try_perf.cpp workload.cpp)

add_executable(dead_loop dead_loop.cpp)

add_executable(preemption preemption.cpp)

add_executable(colocate_threads colocate_threads.cpp)