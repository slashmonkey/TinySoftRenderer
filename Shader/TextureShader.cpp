
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
    out.pos_world = model * in.position;
    out.pos_homo = projection * view * model * in.position;
    out.color = in.color;
    out.normal = invModelMatrix * Vec4f(in.normal);
    out.texcoord = in.texcoord;
    out.rhw = 1.0f / out.pos_homo.w;//w ==-viewspace.z
    return out;
}

Color TextureShader::fragment(const VertexOut& vertex_out) {
    Color tex = texture->sample_bilinear(vertex_out.texcoord);
    return tex;


    //ambient
    Color ambient_color = ambient->get_color() * ambient->get_intensity();

    //diffuse
    Vec3f light_dir = light->get_position() - vertex_out.pos_world;
    float r2 = light_dir.x * light_dir.x + light_dir.y * light_dir.y + light_dir.z * light_dir.z;
    light_dir.normalize();
    float diff = std::max(0.f, light_dir.dot(vertex_out.normal));

    float light_intensity = light->get_intensity();
    Color diffuse_color = light->get_color() * (light_intensity * ( 1.f / r2 ) * diff * material->get_diffuse());

    //specular
    Vec3f eye_dir = (eye_pos - vertex_out.pos_world).normalize();
    Vec3f h = (eye_dir + light_dir).normalize();
    float spec = std::pow(std::max(0.f, h.dot(vertex_out.normal)), material->get_shininess());
    Color specular_color = light->get_color() * light->get_intensity() * spec * material->get_specular();

    return ambient_color +  diffuse_color + specular_color;
}

