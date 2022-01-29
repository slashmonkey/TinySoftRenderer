
#include "Plane.h"

void Plane::update_transform(float deltaTime) {
    shader->set_model_matrix(model);
}

Plane::Plane(std::shared_ptr<IShader>& iShader) : Plane(4, 4, iShader) {}

Plane::Plane(float width, float height, std::shared_ptr<IShader>& iShader) : Mesh(iShader) {
    float half_w = width * 0.5f;
    float half_d = height * 0.5f;

    ind = {
            Vec3i(0, 1, 2), Vec3i(0, 2, 3)
    };

    vertices = {
            Vertex(Vec4f(+half_w, -1.f, -half_d, 1), Red, Vec3f(0.f, 1.f, 0.f), Vec2f(1.f, 1.f)),
            Vertex(Vec4f(-half_w, -1.f, -half_d, 1), Green, Vec3f(0.f, 1.f, 0.f), Vec2f(0.f, 1.f)),
            Vertex(Vec4f(-half_w, -1.f, half_d, 1), Blue, Vec3f(0.f, 1.f, 0.f), Vec2f(0.f, 0.f)),
            Vertex(Vec4f(+half_w, -1.f, half_d, 1), White, Vec3f(0.f, 1.f, 0.f), Vec2f(1.f, 0.f)),
    };
}
