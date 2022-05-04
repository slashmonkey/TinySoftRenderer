
#ifndef SOFTRENDERER_MESH_H
#define SOFTRENDERER_MESH_H
#include "../Geometry.h"
#include "../Shader/IShader.h"
#include <memory>

class Mesh{
public:
    explicit Mesh(std::shared_ptr<IShader>& iShader) : shader(iShader) {};
    virtual ~Mesh() = default;

    std::shared_ptr<IShader> shader;

    int vertex_buf_id = -1;
    int ind_buf_id = -1;

    virtual const std::vector<Vertex>& get_vertices() const { return vertices; };
    virtual const std::vector<Vec3i>& get_ind() const { return ind; };

    virtual void update_transform(float deltaTime) = 0;

protected:
    Mat4f model = Mat4f::identity();
    std::vector<Vertex> vertices;
    std::vector<Vec3i> ind;
};
#endif //SOFTRENDERER_MESH_H
