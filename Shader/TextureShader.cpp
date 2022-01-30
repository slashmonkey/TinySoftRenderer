
#include "TextureShader.h"
#include "../Color.h"
#include <algorithm>
#include <iostream>

void TextureShader::set_model_matrix(const Mat4f& model) {
    SimpleShader::set_model_matrix(model);
    invModelMatrix = model.inverseTranspose();
}

VertexOut TextureShader::vertex(const Vertex& in) {
    VertexOut out;
    out.pos_world = model * in.pos;
    out.pos_homo = projection * view * model * in.pos;
    out.color = in.color;
    out.normal = invModelMatrix * Vec4f(in.normal);
    out.texcoord = in.texcoord;
    out.rhw = 1.0f / out.pos_homo.w;//w ==-viewspace.z
    return out;
}

Color TextureShader::fragment(const VertexOut& vertex_out) {
    Color tex = texture->sample_bilinear(vertex_out.texcoord);
    return tex;
}

