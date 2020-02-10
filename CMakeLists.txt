project(pocker)

cmake_minimum_required(VERSION 3.0.2)

set(SOURCES main.cpp)

add_executable(pocker ${SOURCES})
target_link_libraries(pocker stdc++)
