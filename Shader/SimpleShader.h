
#ifndef SOFTRENDERER_SIMPLESHADER_H
#define SOFTRENDERER_SIMPLESHADER_H

#include "IShader.h"

struct SimpleShader : IShader{
    SimpleShader() = default;
    virtual ~SimpleShader() = default;

    virtual VertexOut vertex(const Vertex& in);
    virtual Color fragment(const VertexOut& vertex_out);
    virtual void set_model_matrix(const Mat4f& model);
    virtual void set_view_matrix(const Mat4f& view);
    virtual void set_proj_matrix(const Mat4f& proj);
};


#endif //SOFTRENDERER_SIMPLESHADER_H
