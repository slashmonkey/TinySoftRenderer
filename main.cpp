#include<iostream>
#include "tga/tgaimage.h"
#include "Math.h"
#include "Geometry.h"
#include "Camera.h"
#include "Rasterizer.h"
#include "Window.h"
#include "Timer.h"

int main(){
    int width = 600;
    int height = 600;

    double delta_time;

    Camera camera(Vec3f(0.0f, 0.0f, 3.0f),Vec3f(0, 0, -5.f),Vec3f (0, 1, 0));
    camera.set_projection(45, (float)width/height, 0.1, 50);
    std::vector<Vertex> vertices{ Vertex(Vec4f(0.f, 1.0f, -1, 1), Red),
                                  Vertex(Vec4f(-1.f, -1.f, -1, 1), Green),
                                  Vertex(Vec4f(1.0f, -1.0f, -1, 1), Blue) };
    std::vector<Vec3i> ind{{0, 1, 2}};

    Rasterizer rasterizer(width, height);
    Vertex_Buf_ID posBufId = rasterizer.set_vertex_buffer(vertices);
    Ind_Buf_ID indBufId = rasterizer.set_index_buffer(ind);

    Mat4f model = Mat4f::identity();

    Window window;
    window.init(width, height);
    Timer timer;

    rasterizer.set_view(camera.get_view_matrix());
    rasterizer.set_proj(camera.get_projection_matrix());
    rasterizer.set_viewport(width, height);

    while (window.is_running){
        timer.start();

        rasterizer.clear(Buffers::Color | Buffers::Depth);

        rasterizer.set_model(model);
        rasterizer.draw(posBufId, indBufId, RenderMode::Fill);

        window.update(rasterizer.output());

        delta_time = timer.elapsed();

        window.set_title(std::to_string((int)(1/delta_time)).c_str());
    }
    window.stop();

    return 0;
}
