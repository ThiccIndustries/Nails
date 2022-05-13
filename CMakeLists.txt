cmake_minimum_required(VERSION 3.20)
project(Nails)

include_directories(${CMAKE_SOURCE_DIR}/Geode-Engine/)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")

add_executable(Nails nails.h n_main.cpp n_rendering.cpp n_ui.cpp n_world.cpp)
target_link_libraries(Nails Geode-Lib)

add_custom_command( TARGET Nails PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${PROJECT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}/resources)