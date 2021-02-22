
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
void Rasterizer::draw_line(const Vertex& begin, const Vertex& end) {
    auto x1 = begin.x();
    auto y1 = begin.y();
    auto x2 = end.x();
    auto y2 = end.y();

    Color line_color(255.f, 255.f, 255.f);

    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;

    dx=x2-x1;
    dy=y2-y1;
    dx1=fabs(dx);
    dy1=fabs(dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;

    if(dy1<=dx1){
        if(dx>=0){
            x=x1; y=y1; xe=x2;
        }else{
            x=x2; y=y2; xe=x1;
        }
        Vec3f point = Vec3f (x, y, 1.0f);
        set_pixel(point, line_color);
        for(i=0;x<xe;i++){
            x=x+1;
            if(px<0){
                px=px+2*dy1;
            }else{
                if((dx<0 && dy<0) || (dx>0 && dy>0)){
                    y=y+1;
                }else{
                    y=y-1;
                }
                px=px+2*(dy1-dx1);
            }
            Vec3f point = Vec3f (x, y, 1.0f);
            set_pixel(point,line_color);
        }
    }
    else{
        if(dy>=0){
            x=x1;
            y=y1;
            ye=y2;
        }else{
            x=x2;
            y=y2;
            ye=y1;
        }
        Vec3f point = Vec3f (x, y, 1.0f);
        set_pixel(point,line_color);
        for(i=0;y<ye;i++){
            y=y+1;
            if(py<=0){
                py=py+2*dx1;
            }else{
                if((dx<0 && dy<0) || (dx>0 && dy>0)){
                    x=x+1;
                }else{
                    x=x-1;
                }
                py=py+2*(dx1-dy1);
            }
            Vec3f point = Vec3f (x, y, 1.0f);
            set_pixel(point,line_color);
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
        //! primitives assembly to triangle.
        Triangle triangle;

        //! vertex shader stage.
        VertexOut vertexOut[] = {
                shader_ptr->vertex(posBuf[0]),
                shader_ptr->vertex(posBuf[1]),
                shader_ptr->vertex(posBuf[2]),
        };

        //! fragment shader stage.
        for(VertexOut& v : vertexOut){
            perspective_division(v);//NDC
            v.pos_homo = viewport * v.pos_homo;//viewport transformation
        }

        Vertex vertices[] = {
                Vertex(vertexOut[0].pos_homo),
                Vertex(vertexOut[1].pos_homo),
                Vertex(vertexOut[2].pos_homo)
        };

        triangle.set_v0(vertices[0]);
        triangle.set_v1(vertices[1]);
        triangle.set_v2(vertices[2]);

        //! rasterization
        if (mode == RenderMode::Wire){
            rasterize_wireframe(triangle);
        }
    }
}

void Rasterizer::rasterize_wireframe(const Triangle& triangle) {
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
