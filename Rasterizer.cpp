
#include <limits>
#include "Rasterizer.h"
#include "iostream"

Rasterizer::Rasterizer(int w, int h) : width(w), height(h) {
    shader_ptr = std::make_shared<SimpleShader>();
    framebuffer_ptr = std::make_shared<FrameBuffer>(width, height);
}

Vertex_Buf_ID Rasterizer::set_vertex_buffer(const std::vector<Vertex>& position) {
    auto id = get_next_id();
    vertex_buf.emplace(id, position);

    return id;
}

Ind_Buf_ID Rasterizer::set_index_buffer(const std::vector<Vec3i>& indices) {
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return id;
}

// Bresenham's line drawing algorithm
void Rasterizer::draw_line(const VertexOut& from, const VertexOut& to) {
    int dx = to.pos_homo.x - from.pos_homo.x;
    int dy = to.pos_homo.y - from.pos_homo.y;
    int stepX = 1, stepY = 1;

    // judge the sign
    if(dx < 0)
    {
        stepX = -1;
        dx = -dx;
    }
    if(dy < 0)
    {
        stepY = -1;
        dy = -dy;
    }

    int d2x = 2*dx, d2y = 2*dy;
    int d2y_minus_d2x = d2y - d2x;
    int sx = from.pos_homo.x;
    int sy = from.pos_homo.y;

    VertexOut tmp;
    // slope < 1.
    if(dy <= dx)
    {
        int flag = d2y - dx;
        for(int i = 0;i <= dx;++ i)
        {
            // linear interpolation
            tmp = lerp(from, to, static_cast<double>(i)/dx);
            // fragment shader
            Vec3f point(sx, sy, 1.f);
            set_pixel(point, shader_ptr->fragment(tmp));
            sx += stepX;
            if(flag <= 0)
                flag += d2y;
            else
            {
                sy += stepY;
                flag += d2y_minus_d2x;
            }
        }
    }
        // slope < 1.
    else
    {
        int flag = d2x - dy;
        for(int i = 0;i <= dy;++ i)
        {
            // linear interpolation
            tmp = lerp(from, to, static_cast<double>(i)/dy);
            // fragment shader
            Vec3f point(sx, sy, 1.f);
            set_pixel(point, shader_ptr->fragment(tmp));
            sy += stepY;
            if(flag <= 0)
                flag += d2x;
            else
            {
                sx += stepX;
                flag -= d2y_minus_d2x;
            }
        }
    }
}

//render pipeline
void Rasterizer::draw(Vertex_Buf_ID posBufId, Ind_Buf_ID indBufId, RenderMode mode) {
    std::vector<Vertex>& posBuf = vertex_buf[posBufId.id];
    std::vector<Vec3i>& indBuf = ind_buf[indBufId.id];

    Mat4f mvp = projection * view * model;

    std::cout << model << std::endl;
    std::cout << view << std::endl;
    std::cout << projection << std::endl;

    for(Vec3i ind : indBuf){
        //! vertex shader stage.
        VertexOut vertexOut[] = {
                shader_ptr->vertex(posBuf[0]),
                shader_ptr->vertex(posBuf[1]),
                shader_ptr->vertex(posBuf[2]),
        };

        for(VertexOut& v : vertexOut){
            perspective_division(v);//NDC
            v.pos_homo = viewport * v.pos_homo;//viewport transformation
        }

        TriangleOut triangleOut;

        triangleOut.set_v0(vertexOut[0]);
        triangleOut.set_v1(vertexOut[1]);
        triangleOut.set_v2(vertexOut[2]);

        //! fragment shader stage. rasterization
        if (mode == RenderMode::Wire){
            rasterize_wireframe(triangleOut);
        }else{

        }
    }
}

void Rasterizer::rasterize_wireframe(const TriangleOut& triangle) {
    draw_line(triangle.get_v0(), triangle.get_v1());
    draw_line(triangle.get_v1(), triangle.get_v2());
    draw_line(triangle.get_v2(), triangle.get_v0());
}

int Rasterizer::get_index(int x, int y) const {
    return y * width + x;
}

Color Rasterizer::get_color(int x, int y) const {
    return framebuffer_ptr->get_color(get_index(x, y));
}

void Rasterizer::set_pixel(const Vec3f& point, const Color& color) {
    if (point.x < 0 || point.x >= width ||
        point.y < 0 || point.y >= height) return;
    int ind = get_index(point.x, point.y);
    framebuffer_ptr->set_pixel(ind, color);
}

void Rasterizer::set_model(const Mat4f& _model) {
    model = _model;
}

void Rasterizer::set_view(const Mat4f& _view) {
    view = _view;
}

void Rasterizer::set_proj(const Mat4f& _proj) {
    projection = _proj;
}

void Rasterizer::set_viewport(const int& width, const int& height) {
    Mat4f _viewport = Mat4f::identity();
    _viewport.set_row(Vec4f(width / 2,        0, 0,  width/2), 0);
    _viewport.set_row(Vec4f(        0,-height/2, 0, height/2), 1);
    _viewport.set_row(Vec4f(        0,        0, 1,        0), 2);
    _viewport.set_row(Vec4f(        0,        0, 0,        1), 3);
    viewport = _viewport;
}

void Rasterizer::perspective_division(VertexOut& vertex) {
    float rhw = 1.0f / vertex.pos_homo.w;
    vertex.pos_homo.x *= rhw;
    vertex.pos_homo.y *= rhw;
    vertex.pos_homo.z *= rhw;
    vertex.pos_homo.w *= 1.0f;
}

void Rasterizer::clear(Buffers buffer) {
    framebuffer_ptr->clear(buffer);
}

VertexOut Rasterizer::lerp(const VertexOut& v1, const VertexOut& v2, float weight) {
    VertexOut vertexOut;
    vertexOut.color = v1.color.lerp(v2.color, weight);
    vertexOut.normal = v1.normal.lerp(v2.normal, weight);
    vertexOut.texcoord = v1.texcoord.lerp(v2.texcoord, weight);
    vertexOut.pos_world = v1.pos_world.lerp(v2.pos_world, weight);
    vertexOut.pos_homo = v1.pos_homo.lerp(v2.pos_homo, weight);
    vertexOut.rhw = v1.rhw * (1-weight) + v2.rhw * weight;
    return vertexOut;
}