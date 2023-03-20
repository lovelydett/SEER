project(exp_yuting_perfmon2)

add_executable(no_pfm no_libpfm.cpp)
target_link_libraries(no_pfm pfm)