
#include<iostream>
#include "tga/tgaimage.h"
#include "Math.h"
#include "Geometry.h"
#include "Camera.h"
#include "Rasterizer.h"

int main(){
    int width = 700;
    int height = 700;
    Rasterizer rasterizer(width, height);

    Camera camera(Vec3f(0, 0, 5),Vec3f(0, 0, -10),Vec3f (0, 1, 0));
    camera.set_projection(45, 1, 0.1, 50);
//    std::vector<Vertex> vertices{ Vertex(0.5, -1, -1), {0, 1, -1}, {-1, 0, -1}};
    std::vector<Vertex> vertices{ Vertex(Vec4f(0.f, 0.8f, -1, 1), Red),
                                  Vertex(Vec4f(-1, -0.5f, -1, 1), Green),
                                  Vertex(Vec4f(0.8, -0.8f, -1, 1), Blue) };
    std::vector<Vec3i> ind{{0, 1, 2}};

    Vertex_Buf_ID posBufId = rasterizer.set_vertex_buffer(vertices);
    Ind_Buf_ID indBufId = rasterizer.set_index_buffer(ind);

    rasterizer.clear(Buffers::Color | Buffers::Depth);

    Mat4f model = Mat4f::identity();

    rasterizer.set_model(model);
    rasterizer.set_view(camera.get_view_matrix());
    rasterizer.set_proj(camera.get_projection_matrix());
    rasterizer.set_viewport(width, height);
    rasterizer.draw(posBufId, indBufId, RenderMode::Fill);

    TGAImage image(width, height, TGAImage::RGB);
    for (int h = 0; h < width; ++h) {
        for (int w = 0; w < height; ++w) {
            Color color = rasterizer.get_color(w, h);
            image.set(w, h, TGAColor(color.R(), color.G(), color.B()));
        }
    }
    image.write_tga_file("output.tga");
    return 0;
}
