
#include "SimpleShader.h"

VertexOut SimpleShader::vertex(const Vertex& in) {
    VertexOut out;
    out.pos_world = model * in.position;
    out.pos_homo = projection * view * model * in.position;
    out.color = in.color;
    out.normal = in.normal;
    out.texcoord = in.texcoord;
    out.rhw = 1.0f / out.pos_homo.w;//w ==-viewspace.z
    return out;
}

Color SimpleShader::fragment(const VertexOut& vertex_out) {
    Color fragColor;
    fragColor = vertex_out.color;
    return fragColor;
}

void SimpleShader::set_model_matrix(const Mat4f& _model) {
    model = _model;
}

void SimpleShader::set_view_matrix(const Mat4f& _view) {
    view = _view;
}

void SimpleShader::set_proj_matrix(const Mat4f& _proj) {
    projection = _proj;
}
