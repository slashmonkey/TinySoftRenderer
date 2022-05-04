cmake_minimum_required(VERSION 3.7)
project(SoftRenderer)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/local/include)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(SoftRenderer SoftRender/main.cpp SoftRender/Math.h Dependencies/tga/tgaimage.h Dependencies/tga/tgaimage.cpp SoftRender/Geometry.h SoftRender/Color.h SoftRender/Rasterizer.cpp SoftRender/Rasterizer.h SoftRender/Camera.cpp SoftRender/Camera.h SoftRender/FrameBuffer.cpp SoftRender/FrameBuffer.h SoftRender/Shader/IShader.h SoftRender/Shader/SimpleShader.cpp SoftRender/Shader/SimpleShader.h SoftRender/Window.cpp SoftRender/Window.h SoftRender/Timer.h SoftRender/Mesh/Triangle.cpp SoftRender/Mesh/Triangle.h SoftRender/Mesh/Mesh.h SoftRender/Mesh/Box.cpp SoftRender/Mesh/Box.h SoftRender/Light.h SoftRender/Shader/PhongShader.cpp SoftRender/Shader/PhongShader.h SoftRender/Shader/GouraudShader.cpp SoftRender/Shader/GouraudShader.h SoftRender/Mesh/Plane.cpp SoftRender/Mesh/Plane.h Dependencies/stb_image.h SoftRender/Texture2D.cpp SoftRender/Texture2D.h SoftRender/Shader/TextureShader.cpp SoftRender/Shader/TextureShader.h SoftRender/Mesh/ObjModel.cpp SoftRender/Mesh/ObjModel.h)
target_link_libraries(SoftRenderer ${SDL2_LIBRARIES})