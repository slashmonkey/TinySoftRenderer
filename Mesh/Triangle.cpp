
#include "Triangle.h"

void Triangle::update_transform(float deltaTime){
    angle += 45 * deltaTime;
//    model.set_rotationY(angle);
    shader->set_model_matrix(model);
}

Triangle::Triangle(std::shared_ptr<IShader>& iShader) : Mesh(iShader) {
    ind = {Vec3i(0, 1, 2)};
    vertices = { Vertex(Vec4f(1.f, 1.0f, -8.0, 1), Red),
                 Vertex(Vec4f(-1.f, -.5f, -5.0, 1), Green),
                 Vertex(Vec4f(1.0f, -0.7f, 0., 1), Blue) };
}
