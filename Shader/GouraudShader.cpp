
#include "GouraudShader.h"

void GouraudShader::set_model_matrix(const Mat4f& model) {
    SimpleShader::set_model_matrix(model);
    invModelMatrix = model.inverseTranspose();
}

VertexOut GouraudShader::vertex(const Vertex& in) {
    VertexOut out;
    out.pos_world = model * in.pos;
    out.pos_homo = projection * view * model * in.pos;
    out.color = in.color;
    out.normal = invModelMatrix * Vec4f(in.normal);
    out.texcoord = in.texcoord;
    out.rhw = 1.0f / out.pos_homo.w;//w ==-viewspace.z

    //lighting
    Color ambient_color = ambient->get_color() * ambient->get_intensity();

    //diffuse
    Vec3f light_dir = light->get_position() - out.pos_world;
    float r2 = light_dir.x * light_dir.x + light_dir.y * light_dir.y + light_dir.z * light_dir.z;
    light_dir.normalize();
    float diff = std::max(0.f, light_dir.dot(out.normal));

    float light_intensity = light->get_intensity();
    Color diffuse_color = light->get_color() * (light_intensity * ( 1.f / r2 ) * diff * material->get_diffuse());

    //specular
    Vec3f eye_dir = (*eye_pos - out.pos_world).normalize();
    Vec3f h = (eye_dir + light_dir).normalize();
    float spec = std::pow(std::max(0.f, h.dot(out.normal)), material->get_shininess());
    Color specular_color = light->get_color() * light->get_intensity() * spec * material->get_specular();

    out.color = ambient_color +  diffuse_color + specular_color;


    return out;
}

//Color is interpolated in rasterizer
Color GouraudShader::fragment(const VertexOut& vertex_out) {
    return SimpleShader::fragment(vertex_out);
}

