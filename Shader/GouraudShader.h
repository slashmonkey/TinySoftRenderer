
#ifndef SOFTRENDERER_GOURAUDSHADER_H
#define SOFTRENDERER_GOURAUDSHADER_H

#include "SimpleShader.h"
#include "../Light.h"

struct GouraudShader : public SimpleShader {
    GouraudShader() = default;
    ~GouraudShader() override = default;

    void set_material(const Material* _material){ material = _material; }
    void set_ambient(const Ambient* _ambient){ ambient = _ambient;}
    void set_light(const Light* _light) { light = _light; }
    void set_eye_pos(const Vec3f* _eye_pos) { eye_pos = _eye_pos; }

private:
    const Material* material;
    const Ambient* ambient;
    const Light* light;
    const Vec3f* eye_pos;

    Mat4f invModelMatrix;

    VertexOut vertex(const Vertex& in) override;
    Color fragment(const VertexOut& vertex_out) override;

    void set_model_matrix(const Mat4f& model) override;
};


#endif //SOFTRENDERER_GOURAUDSHADER_H
