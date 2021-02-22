
#include "SimpleShader.h"

VertexOut SimpleShader::vertex(const Vertex& in) {
    VertexOut out;
    out.pos_world = in.pos;
    out.pos_homo = in.pos;
    out.color = in.color;
    out.normal = in.normal;
    out.rhw = 1.0;
    out.texcoord = in.texcoord;
    return out;
}

void SimpleShader::set_model_matrix(const Mat4f& model) {

}

void SimpleShader::set_view_matrix(const Mat4f& view) {

}

void SimpleShader::set_proj_matrix(const Mat4f& proj) {

}
