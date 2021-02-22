cmake_minimum_required(VERSION 3.17)
project(SoftRenderer)

set(CMAKE_CXX_STANDARD 11)
find_package(Eigen3 REQUIRED)
include_directories(EIGEN3_INCLUDE_DIR)
include_directories(/usr/local/include)

add_executable(SoftRenderer main.cpp Math.h tga/tgaimage.h tga/tgaimage.cpp Geometry.h Color.h Rasterizer.cpp Rasterizer.h Camera.cpp Camera.h FrameBuffer.cpp FrameBuffer.h Shader/IShader.h Shader/SimpleShader.cpp Shader/SimpleShader.h Mesh.cpp Mesh.h)