
#ifndef SOFTRENDERER_TEXTURESHADER_H
#define SOFTRENDERER_TEXTURESHADER_H

#include "SimpleShader.h"
#include "../Light.h"
#include "../Texture2D.h"

struct TextureShader : public SimpleShader{
public:
    TextureShader() = default;
    ~TextureShader() override = default;

    void set_material(const Material* _material){ material = _material; }
    void set_ambient(const Ambient* _ambient){ ambient = _ambient;}
    void set_light(const Light* _light) { light = _light; }
    void set_eye_pos(Vec3f _eye_pos) { eye_pos = _eye_pos; }

    void set_texture(const Texture2D* tex) { texture = tex; }

private:
    const Material* material;
    const Ambient* ambient;
    const Light* light;
    Vec3f eye_pos;

    const Texture2D* texture;

    Mat4f invModelMatrix;

    VertexOut vertex(const Vertex& in) override;
    Color fragment(const VertexOut& vertex_out) override;

    void set_model_matrix(const Mat4f& model) override;
};


#endif //SOFTRENDERER_TEXTURESHADER_H
