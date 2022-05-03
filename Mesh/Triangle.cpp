
#include "Triangle.h"

void Triangle::update_transform(float deltaTime){
    angle += 45 * deltaTime;
//    model.set_rotationY(angle);
    shader->set_model_matrix(model);
}

Triangle::Triangle(std::shared_ptr<IShader>& iShader) : Mesh(iShader) {
    ind = {Vec3i(0, 1, 2)};
    vertices = { Vertex(Vec4f(1.0f, -0.5f, 5.0f, 1), Red),
                 Vertex(Vec4f(0.f, 0.f, -4.0f, 1), Green),
                 Vertex(Vec4f(-1.0f, -0.5f, 5.f, 1), Blue) };
}
