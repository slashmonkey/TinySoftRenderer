
#include <limits>
#include "Rasterizer.h"
#include "iostream"

Rasterizer::Rasterizer(int w, int h) : width(w), height(h) {
    shader_ptr = nullptr;
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

    for(Vec3i ind : indBuf){
        // vertex shader stage.
        VertexOut vertexOut[] = {
            shader_ptr->vertex(posBuf[ind[0]]),
            shader_ptr->vertex(posBuf[ind[1]]),
            shader_ptr->vertex(posBuf[ind[2]]),
        };

        // backface culling
        if(backface_culling(vertexOut[0], vertexOut[1], vertexOut[2])) continue;

        TriangleOut triangleOut(vertexOut[0], vertexOut[1], vertexOut[2]);
        std::vector<TriangleOut> triangleList = {triangleOut};
        //Homo Clipping
        homo_clipping(triangleList, Vec4f(0.f, 0.f, 1.f, 1.f), Vec4f(0.f, 0.f, 1.f, 0.f));//near plane clipping
        for (TriangleOut& triangle: triangleList) {
            VertexOut v0 = triangle.get_v0();
            VertexOut v1 = triangle.get_v1();
            VertexOut v2 = triangle.get_v2();

            perspective_division(v0);//NDC
            v0.pos_homo = viewport * v0.pos_homo;//viewport transformation

            perspective_division(v1);
            v1.pos_homo = viewport * v1.pos_homo;

            perspective_division(v2);
            v2.pos_homo = viewport * v2.pos_homo;

            triangle.set_v0(v0);
            triangle.set_v1(v1);
            triangle.set_v2(v2);

            // fragment shader stage. rasterization
            if (mode == RenderMode::Wire){
                rasterize_wireframe(triangle);
            }else{
                barycentric_fill(triangle);
//                edge_walking_fill(triangle);
            }
        }
    }
}

static bool inside_triangle(float alpha, float beta, float gamma)
{
    int flag = 0;
    // here epsilon is to alleviate precision bug
    if (alpha > -EPSILON && beta > -EPSILON && gamma > -EPSILON)
        flag = 1;

    return flag;
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const TriangleOut& triangle)
{
    VertexOut v[3] = {triangle.get_v0(), triangle.get_v1(), triangle.get_v2()};
    float c1 = (x*(v[1].pos_homo.y - v[2].pos_homo.y) + (v[2].pos_homo.x - v[1].pos_homo.x)*y + v[1].pos_homo.x*v[2].pos_homo.y - v[2].pos_homo.x*v[1].pos_homo.y) /
            (v[0].pos_homo.x*(v[1].pos_homo.y - v[2].pos_homo.y) + (v[2].pos_homo.x - v[1].pos_homo.x)*v[0].pos_homo.y + v[1].pos_homo.x*v[2].pos_homo.y - v[2].pos_homo.x*v[1].pos_homo.y);
    float c2 = (x*(v[2].pos_homo.y - v[0].pos_homo.y) + (v[0].pos_homo.x - v[2].pos_homo.x)*y + v[2].pos_homo.x*v[0].pos_homo.y - v[0].pos_homo.x*v[2].pos_homo.y) /
            (v[1].pos_homo.x*(v[2].pos_homo.y - v[0].pos_homo.y) + (v[0].pos_homo.x - v[2].pos_homo.x)*v[1].pos_homo.y + v[2].pos_homo.x*v[0].pos_homo.y - v[0].pos_homo.x*v[2].pos_homo.y);
    float c3 = (x*(v[0].pos_homo.y - v[1].pos_homo.y) + (v[1].pos_homo.x - v[0].pos_homo.x)*y + v[0].pos_homo.x*v[1].pos_homo.y - v[1].pos_homo.x*v[0].pos_homo.y) /
            (v[2].pos_homo.x*(v[0].pos_homo.y - v[1].pos_homo.y) + (v[1].pos_homo.x - v[0].pos_homo.x)*v[2].pos_homo.y + v[0].pos_homo.x*v[1].pos_homo.y - v[1].pos_homo.x*v[0].pos_homo.y);
    return {c1,c2,c3};
}

void Rasterizer::barycentric_fill(const TriangleOut& triangleOut) {
    VertexOut v0 = triangleOut.get_v0();
    VertexOut v1 = triangleOut.get_v1();
    VertexOut v2 = triangleOut.get_v2();

    float x_min = std::floor(std::min(v0.pos_homo.x, std::min(v1.pos_homo.x, v2.pos_homo.x)));
    float x_max = std::ceil(std::max(v0.pos_homo.x, std::max(v1.pos_homo.x, v2.pos_homo.x)));
    float y_min = std::floor(std::min(v0.pos_homo.y, std::min(v1.pos_homo.y, v2.pos_homo.y)));
    float y_max = std::ceil(std::max(v0.pos_homo.y, std::max(v1.pos_homo.y, v2.pos_homo.y)));

    VertexOut current;
    for (int y = y_min; y < y_max; ++y) {
        for (int x = x_min; x < x_max; ++x) {
            auto[alpha, beta, gamma] = computeBarycentric2D(x + 0.5f, y + 0.5f, triangleOut);
            if (inside_triangle(alpha, beta, gamma)){
                int ind = get_index(x, y);
                //z is interpolated view space depth for the current pixel
                float z = 1 / (alpha * triangleOut.get_v0().rhw + beta * triangleOut.get_v1().rhw + gamma * triangleOut.get_v2().rhw);
                if(z < framebuffer_ptr->get_depth(ind)){
                    current = interpolated_barycentric(triangleOut, alpha, beta, gamma);
                    framebuffer_ptr->set_depth(ind, z);
                    Vec3f point(x, y, z);
                    set_pixel(point, shader_ptr->fragment(current));
                }
            }
        }
    }
}

void Rasterizer::rasterize_wireframe(const TriangleOut& triangle) {
    draw_line(triangle.get_v0(), triangle.get_v1());
    draw_line(triangle.get_v1(), triangle.get_v2());
    draw_line(triangle.get_v2(), triangle.get_v0());
}

void Rasterizer::scan_line_per_row(const VertexOut &left, const VertexOut &right)
{
    VertexOut current;
    int length = right.pos_homo.x - left.pos_homo.x + 1;
    for(int i = 0;i <= length;++i)
    {
        // linear interpolation
        double weight = static_cast<double>(i)/length;
        current = lerp(left, right, weight);
        current.pos_homo.x = left.pos_homo.x + i;
        current.pos_homo.y = left.pos_homo.y;
        // fragment shade
        Vec3f point(current.pos_homo.x, current.pos_homo.y, 1.f);
        set_pixel(point, shader_ptr->fragment(current));
    }
}

void Rasterizer::rasterize_top_triangle(VertexOut &v1, VertexOut &v2, VertexOut &v3)
{
    VertexOut left = v2;
    VertexOut right = v3;
    VertexOut dest = v1;
    VertexOut tmp, newleft, newright;
    if(left.pos_homo.x > right.pos_homo.x)
    {
        tmp = left;
        left = right;
        right = tmp;
    }
    int dy = left.pos_homo.y - dest.pos_homo.y + 1;

    for(int i = 0;i < dy;++i)
    {
        double weight = 0;
        if(dy != 0)
            weight = static_cast<double>(i)/dy;
        newleft = lerp(left, dest, weight);
        newright = lerp(right, dest, weight);
        newleft.pos_homo.y = newright.pos_homo.y = left.pos_homo.y - i;
        scan_line_per_row(newleft, newright);
    }
}

void Rasterizer::rasterize_bottom_triangle(VertexOut &v1, VertexOut &v2, VertexOut &v3)
{
    VertexOut left = v1;
    VertexOut right = v2;
    VertexOut dest = v3;
    VertexOut tmp, newleft, newright;
    if(left.pos_homo.x > right.pos_homo.x)
    {
        tmp = left;
        left = right;
        right = tmp;
    }
    int dy = dest.pos_homo.y - left.pos_homo.y + 1;


    for(int i = 0;i < dy;++i)
    {
        double weight = 0;
        if(dy != 0)
            weight = static_cast<double>(i)/dy;
        newleft = lerp(left, dest, weight);
        newright = lerp(right, dest, weight);
        newleft.pos_homo.y = newright.pos_homo.y = left.pos_homo.y + i;
        scan_line_per_row(newleft, newright);
    }
}

void Rasterizer::edge_walking_fill(const TriangleOut& triangleOut)
{
    // split the triangle into two part
    VertexOut tmp;
    VertexOut target[3] = {triangleOut.get_v0(), triangleOut.get_v1(), triangleOut.get_v2()};
    if(target[0].pos_homo.y > target[1].pos_homo.y)
    {
        tmp = target[0];
        target[0] = target[1];
        target[1] = tmp;
    }
    if(target[0].pos_homo.y > target[2].pos_homo.y)
    {
        tmp = target[0];
        target[0] = target[2];
        target[2] = tmp;
    }
    if(target[1].pos_homo.y > target[2].pos_homo.y)
    {
        tmp = target[1];
        target[1] = target[2];
        target[2] = tmp;
    }

    // bottom triangle
    if(equal(target[0].pos_homo.y,target[1].pos_homo.y))
    {
        rasterize_bottom_triangle(target[0], target[1], target[2]);
    }
        // top triangle
    else if(equal(target[1].pos_homo.y,target[2].pos_homo.y))
    {
        rasterize_top_triangle(target[0], target[1], target[2]);
    }
        // split it.
    else
    {
        double weight = static_cast<double>(target[1].pos_homo.y-target[0].pos_homo.y)/(target[2].pos_homo.y-target[0].pos_homo.y);
        VertexOut newPoint = lerp(target[0],target[2],weight);
        newPoint.pos_homo.y = target[1].pos_homo.y;
        rasterize_top_triangle(target[0], newPoint, target[1]);
        rasterize_bottom_triangle(newPoint, target[1], target[2]);
    }

}

int Rasterizer::get_index(int x, int y) const {
    return (height-1-y)*width + x;
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

void Rasterizer::set_view(const Mat4f& _view) {
    view = _view;
}

void Rasterizer::set_projection(const Mat4f& _proj) {
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
    float rhw = vertex.rhw;
    vertex.pos_homo.x *= rhw;
    vertex.pos_homo.y *= rhw;
    vertex.pos_homo.z *= rhw;
    vertex.pos_homo.w = 1.0f;
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
    vertexOut.rhw = 1.0f/vertexOut.pos_homo.w;
    return vertexOut;
}

VertexOut Rasterizer::interpolated_barycentric(const TriangleOut& triangle, float alpha, float beta, float gamma) {
    //perspective correction
    float v0_rhw = triangle.get_v0().rhw;
    float v1_rhw = triangle.get_v1().rhw;
    float v2_rhw = triangle.get_v2().rhw;
    float z = 1 / (v0_rhw * alpha + v1_rhw * beta + v2_rhw * gamma);

    VertexOut vertexOut;
    vertexOut.color = ( triangle.get_v0().color * v0_rhw  * alpha +
                        triangle.get_v1().color * v1_rhw * beta +
                        triangle.get_v2().color * v2_rhw * gamma ) * z;

    vertexOut.normal = ( triangle.get_v0().normal * v0_rhw  * alpha +
                         triangle.get_v1().normal * v1_rhw * beta +
                         triangle.get_v2().normal * v2_rhw * gamma ) * z;

    vertexOut.texcoord = ( triangle.get_v0().texcoord * v0_rhw  * alpha +
                           triangle.get_v1().texcoord * v1_rhw * beta +
                           triangle.get_v2().texcoord * v2_rhw * gamma ) * z;

    vertexOut.pos_world = (triangle.get_v0().pos_world * v0_rhw * alpha +
                          triangle.get_v1().pos_world * v1_rhw * beta +
                          triangle.get_v2().pos_world * v2_rhw * gamma) * z;

    return vertexOut;
}

void Rasterizer::add_mesh(Mesh* mesh) {
    meshes.push_back(mesh);
    mesh->vertex_buf_id = set_vertex_buffer(mesh->get_vertices()).id;
    mesh->ind_buf_id = set_index_buffer(mesh->get_ind()).id;
}

void Rasterizer::set_shader(const std::shared_ptr<IShader>& iShader) {
    shader_ptr = iShader;

    shader_ptr->set_view_matrix(view);
    shader_ptr->set_proj_matrix(projection);
}

bool Rasterizer::backface_culling(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3) const {
    const Vec3f& v1_pos = v1.pos_world;
    const Vec3f& v2_pos = v2.pos_world;
    const Vec3f& v3_pos = v3.pos_world;

    Vec3f v12 = v2_pos - v1_pos;
    Vec3f v23 = v3_pos - v2_pos;

    Vec3f crs = v12.cross(v23);
    Vec3f dir = eye_pos - v1_pos;

    //CCW as Front Face
    return (dir.dot(crs) <= 0);
}

void Rasterizer::set_eye_pos(const Vec3f& _eye_pos) {
    eye_pos = _eye_pos;
}

void Rasterizer::homo_clipping(std::vector<TriangleOut>& triangle_list, const Vec4f& plane, const Vec4f& normal) {
    for (auto it = triangle_list.begin(); it != triangle_list.end();) {
        TriangleOut& tri = *it;
        VertexOut v0 = tri.get_v0();
        VertexOut v1 = tri.get_v1();
        VertexOut v2 = tri.get_v2();

        float d0 = plane.dot(v0.pos_homo);
        float d1 = plane.dot(v1.pos_homo);
        float d2 = plane.dot(v2.pos_homo);

        bool out0 = d0 < 0;
        bool out1 = d1 < 0;
        bool out2 = d2 < 0;

        std::vector<float> vert_dis = {d0 , d1 , d2};
        std::vector<float> vert_dis_positive = {std::abs(d0), std::abs(d1), std::abs(d2)};
        std::vector<bool> out_list = {out0 , out1 , out2};
        int out_count = 0;
        for (int i = 0; i < out_list.size() ; ++ i) {
            bool out = out_list[i];
            out_count = out_count + (out ? 1 : 0);
        }

        std::vector<VertexOut> vertices = {v0, v1 , v2};

        if (out_count == 0){
            ++it;
            continue;
        }else if (out_count == 2){
            //form a triangle
            std::vector<int> out_indices;
            int in_vertex_idx = 0;
            for (int i = 0; i < out_list.size(); ++ i) {
                if (out_list[i]) {
                    out_indices.push_back(i);
                } else {
                    in_vertex_idx = i;
                }
            }

            VertexOut &in_vert = vertices.at(in_vertex_idx);

            for (int i = 0; i < out_indices.size(); ++i) {
                int index = out_indices[i];
                VertexOut& out_vert = vertices.at(index);
                float out_dis = vert_dis_positive[index];
                float in_dis = vert_dis_positive[in_vertex_idx];
                float factor = in_dis / (in_dis + out_dis);
                VertexOut new_vert = lerp(in_vert, out_vert, factor);
                vertices[index] = new_vert;
            }

            tri.set_v0(vertices[0]);
            tri.set_v1(vertices[1]);
            tri.set_v2(vertices[2]);
            ++it;
        }else if(out_count == 1){
            //a quad to form two triangles
            std::vector<int> in_indices;
            int out_vertex_idx = 0;
            for (int i = 0; i < out_list.size(); ++ i) {
                if (!out_list[i]) {
                    in_indices.push_back(i);
                } else {
                    out_vertex_idx = i;
                }
            }
            VertexOut& out_vert = vertices.at(out_vertex_idx);
            std::vector<VertexOut> vert_new_list;
            for (int i = 0; i < in_indices.size(); ++i) {
                int index = in_indices[i];
                VertexOut in_vert = vertices[index];
                float in_dis = vert_dis_positive[index];
                float out_dis = vert_dis_positive[out_vertex_idx];
                float factor = in_dis / (in_dis + out_dis);
                VertexOut new_vert = lerp(in_vert, out_vert, factor);
                vert_new_list.push_back(in_vert);
                vert_new_list.push_back(new_vert);
            }
            tri.set_v0(vert_new_list[0]);
            tri.set_v1(vert_new_list[1]);
            tri.set_v2(vert_new_list[2]);

            TriangleOut triNew(vert_new_list[1] , vert_new_list[2] , vert_new_list[3]);
            it = triangle_list.insert(it + 1, triNew);
            ++it;

        }else if (out_count == 3) {
            it = triangle_list.erase(it);
        }
    }
}
