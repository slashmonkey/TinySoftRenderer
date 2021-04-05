
#include "Box.h"

void Box::update_transform(float deltaTime) {
    angle += 45 * deltaTime;
    model.set_rotationY(angle);
    shader->set_model_matrix(model);
}

Box::Box(float width, float height, float depth, std::shared_ptr<IShader>& iShader) : Mesh(iShader) {
    float half_w = width * 0.5f;
    float half_h = height * 0.5f;
    float half_d = depth * 0.5f;
    ind = {
            //front
            Vec3i(0, 1, 2), Vec3i(0, 2, 3),
            //left
            Vec3i(4, 5, 6), Vec3i(4, 6, 7),
            //back
            Vec3i(8, 9, 10), Vec3i(8, 10, 11),
            //right
            Vec3i(12, 13, 14), Vec3i(12, 14, 15),
            //top
            Vec3i(16, 17, 18), Vec3i(16, 18, 19),
            //down
            Vec3i(20, 21, 22), Vec3i(20, 22, 23)
    };
    vertices = {
         //front
         Vertex(Vec4f(half_w, half_h, half_d, 1), Red),
         Vertex(Vec4f(-half_w, half_h, half_d, 1), Green),
         Vertex(Vec4f(-half_w,-half_h, half_d, 1), Blue),
         Vertex(Vec4f(half_w, -half_h, half_d, 1), White),
         //left
         Vertex(Vec4f(-half_w, +half_h, half_d, 1), Red),
         Vertex(Vec4f(-half_w, +half_h, -half_d, 1), Green),
         Vertex(Vec4f(-half_w, -half_h, -half_d, 1), Blue),
         Vertex(Vec4f(-half_w, -half_h, half_d, 1), White),
         //back
         Vertex(Vec4f(-half_w, +half_h, -half_d, 1), Red),
         Vertex(Vec4f(+half_w, +half_h, -half_d, 1), Green),
         Vertex(Vec4f(+half_w, -half_h, -half_d, 1), Blue),
         Vertex(Vec4f(-half_w, -half_h, -half_d, 1), White),
         //right
         Vertex(Vec4f(half_w, +half_h, -half_d, 1), Red),
         Vertex(Vec4f(half_w, +half_h, +half_d, 1), Green),
         Vertex(Vec4f(half_w, -half_h, +half_d, 1), Blue),
         Vertex(Vec4f(half_w, -half_h, -half_d, 1), White),
         //top
         Vertex(Vec4f(+half_w, half_h, -half_d, 1), Red),
         Vertex(Vec4f(-half_w, half_h, -half_d, 1), Green),
         Vertex(Vec4f(-half_w, half_h, half_d, 1), Blue),
         Vertex(Vec4f(+half_w, half_h, half_d, 1), White),
         //down
         Vertex(Vec4f(+half_w, -half_h, -half_d, 1), Red),
         Vertex(Vec4f(+half_w, -half_h, +half_d, 1), Green),
         Vertex(Vec4f(-half_w, -half_h, +half_d, 1), Blue),
         Vertex(Vec4f(-half_w, -half_h, -half_d, 1), White)
    };
}

Box::Box(std::shared_ptr<IShader>& iShader) : Box(1, 1, 1, iShader){}


