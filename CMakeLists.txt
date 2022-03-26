cmake_minimum_required(VERSION 3.22)

project(quickhull)

link_libraries(-l\"${CMAKE_SOURCE_DIR}\"/libs/glew32s.lib)
link_libraries(-l\"${CMAKE_SOURCE_DIR}\"/libs/glfw3_mt.lib)

add_executable(${PROJECT_NAME} quickhull.cpp)