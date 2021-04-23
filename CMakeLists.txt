cmake_minimum_required(VERSION 3.12)
project(Main VERSION 0.0.0)

add_compile_options(/MP)
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

add_subdirectory(framework)
add_subdirectory(graduate_work)