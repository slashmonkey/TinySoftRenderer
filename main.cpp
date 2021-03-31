#include<iostream>
#include "tga/tgaimage.h"
#include "Math.h"
#include "Geometry.h"
#include "Camera.h"
#include "Rasterizer.h"
#include "Window.h"

int main(){
    int width = 800;
    int height = 600;
    Rasterizer rasterizer(width, height);

    Camera camera(Vec3f(0.0f, 0.0f, 3.0f),Vec3f(0, 0, -5.f),Vec3f (0, 1, 0));
    camera.set_projection(45, (float)width/height, 0.1, 50);
    std::vector<Vertex> vertices{ Vertex(Vec4f(0.f, 1.0f, -1, 1), Red),
                                  Vertex(Vec4f(-1, -0.5f, -1, 1), Green),
                                  Vertex(Vec4f(0.8, -0.8f, -1, 1), Blue) };
    std::vector<Vec3i> ind{{0, 1, 2}};

    Vertex_Buf_ID posBufId = rasterizer.set_vertex_buffer(vertices);
    Ind_Buf_ID indBufId = rasterizer.set_index_buffer(ind);

    Mat4f model = Mat4f::identity();

    rasterizer.clear(Buffers::Color | Buffers::Depth);

    rasterizer.set_model(model);
    rasterizer.set_view(camera.get_view_matrix());
    rasterizer.set_proj(camera.get_projection_matrix());
    rasterizer.set_viewport(width, height);
    rasterizer.draw(posBufId, indBufId, RenderMode::Fill);

    {
        TGAImage image(width, height, TGAImage::RGB);
        for (int h = 0; h < width; ++h) {
            for (int w = 0; w < height; ++w) {
                Color color = rasterizer.get_color(h, w);
                image.set(h, w, TGAColor(color.R(), color.G(), color.B()));
            }
        }
        image.write_tga_file("output.tga");
    }

    Window window;
    window.init(width, height);
    window.update(rasterizer.output());
    window.stop();

    return 0;
}
