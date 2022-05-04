
#ifndef SOFTRENDERER_RASTERIZER_H
#define SOFTRENDERER_RASTERIZER_H

#include "Math.h"
#include "Geometry.h"
#include "FrameBuffer.h"
#include "Mesh/Mesh.h"
#include "Shader/SimpleShader.h"
#include "Light.h"
#include <map>
#include <memory>

struct Vertex_Buf_ID
{
    int id = 0;
    Vertex_Buf_ID(int i){ id = i;}
};

struct Ind_Buf_ID
{
    int id = 0;
    Ind_Buf_ID(int i){ id = i;}
};

enum class Primitive
{
    Line,
    Triangle
};

enum class RenderMode{
    Wire,
    Fill
};

class Rasterizer {
public:
    Rasterizer(int w, int h);

    Color* output() const { return framebuffer_ptr->get_color_buffer();}
    Color get_color(int x, int y) const;
    Vertex_Buf_ID set_vertex_buffer(const std::vector<Vertex>& position);
    Ind_Buf_ID set_index_buffer(const std::vector<Vec3i>& indices);
    void set_shader(const std::shared_ptr<IShader>& iShader);
    void add_mesh(Mesh* mesh);
    const std::vector<Mesh*>& get_meshes() const { return meshes;}

public:
    void set_pixel(const Vec3f& point, const Color& color);
    void clear(Buffers buffer);
    void draw(Vertex_Buf_ID posBufId, Ind_Buf_ID indBufId, RenderMode mode);
    VertexOut lerp(const VertexOut& v1, const VertexOut& v2, float weight);

    void set_view(const Mat4f& _view);
    void set_projection(const Mat4f& _proj);
    void set_viewport(const int& width, const int& height);
    void perspective_division(VertexOut& vertex);

    void set_near_far(float _near, float _far) { near = _near; far = _far; }
    void set_eye_pos(const Vec3f& _eye_pos);

private:
    // Bresenham's line drawing algorithm
    void draw_line(const VertexOut& begin, const VertexOut& end);
    void rasterize_wireframe(const TriangleOut& triangle);

    void edge_walking_fill(const TriangleOut& triangleOut);
    void scan_line_per_row(const VertexOut &left, const VertexOut &right);
    void rasterize_top_triangle(VertexOut &v1, VertexOut &v2, VertexOut &v3);
    void rasterize_bottom_triangle(VertexOut &v1, VertexOut &v2, VertexOut &v3);

    //Barycentric
    void barycentric_fill(const TriangleOut& triangleOut);
    VertexOut interpolated_barycentric(const TriangleOut& triangle, float alpha, float beta, float gamma);

    //CCW back face culling
    bool backface_culling(const VertexOut &v1, const VertexOut &v2, const VertexOut &v3) const;

    //clipping in homogeneous space
    void homo_clipping(std::vector<TriangleOut>& triangle_list, const Vec4f& plane, const Vec4f& normal);

private:
    std::shared_ptr<FrameBuffer> framebuffer_ptr;
    std::shared_ptr<IShader> shader_ptr;
    std::vector<Mesh*> meshes;

    Mat4f view;
    Mat4f projection;
    Mat4f viewport;

    int width, height;
    int next_id = 0;

    std::map<int, std::vector<Vertex>> vertex_buf;
    std::map<int, std::vector<Vec3i>> ind_buf;

    int get_index(int x, int y) const;
    int get_next_id() { return next_id++; }

    Vec3f eye_pos;
    float near;
    float far;
};


#endif //SOFTRENDERER_RASTERIZER_H
