
#ifndef SOFTRENDERER_TRIANGLE_H
#define SOFTRENDERER_TRIANGLE_H

#include "Mesh.h"
#include "../Geometry.h"

class Triangle : public Mesh{
public:
    Triangle(std::shared_ptr<IShader>& iShader);
    ~Triangle() = default;

    void update_transform(float deltaTime) override;

private:
    float angle = 0.0;
};


#endif //SOFTRENDERER_TRIANGLE_H
