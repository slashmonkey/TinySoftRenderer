cmake_minimum_required(VERSION 3.7)
project(SoftRenderer)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/local/include)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(SoftRenderer main.cpp Math.h tga/tgaimage.h tga/tgaimage.cpp Geometry.h Color.h Rasterizer.cpp Rasterizer.h Camera.cpp Camera.h FrameBuffer.cpp FrameBuffer.h Shader/IShader.h Shader/SimpleShader.cpp Shader/SimpleShader.h Mesh.cpp Mesh.h Window.cpp Window.h Timer.h)
target_link_libraries(SoftRenderer ${SDL2_LIBRARIES})