cmake_minimum_required(VERSION 3.22)
project(GraphicsProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(GraphicsProject main.cpp)

target_link_libraries(GraphicsProject -lopengl32 -lglew32 -lfreeglut)
