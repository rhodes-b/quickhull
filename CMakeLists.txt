cmake_minimum_required(VERSION 3.1)

project(quickhull)

if (WIN32)
link_libraries(-l\"${CMAKE_SOURCE_DIR}\"/libs/windows/glew32s.lib)
link_libraries(-l\"${CMAKE_SOURCE_DIR}\"/libs/windows/glfw3_mt.lib)
endif (WIN32)

if (UNIX)

link_directories(./libs/linux)
link_libraries(-lGLEW)
link_libraries(-lglfw3)

link_libraries(-ldl)
link_libraries(-lGL)
link_libraries(-lX11)
link_libraries(-lpthread)

endif (UNIX)

add_executable(${PROJECT_NAME} quickhull.cpp)
