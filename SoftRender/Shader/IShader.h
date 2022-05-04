
#ifndef SOFTRENDERER_ISHADER_H
#define SOFTRENDERER_ISHADER_H

#include "../Geometry.h"
#include "../Math.h"

struct IShader{
    IShader() = default;
    virtual ~IShader() = default;

    virtual VertexOut vertex(const Vertex& vertex_in) = 0;
    virtual Color fragment(const VertexOut& vertex_out) = 0;
    virtual void set_model_matrix(const Mat4f& model) = 0;
    virtual void set_view_matrix(const Mat4f& view) = 0;
    virtual void set_proj_matrix(const Mat4f& proj) = 0;
};

#endif //SOFTRENDERER_ISHADER_H
