
#ifndef SOFTRENDERER_PLANE_H
#define SOFTRENDERER_PLANE_H

#include "Mesh.h"

class Plane : public Mesh{
public:
    Plane(std::shared_ptr<IShader>& iShader);
    Plane(float width, float height, std::shared_ptr<IShader>& iShader);
    ~Plane() = default;

    void update_transform(float deltaTime) override;
};


#endif //SOFTRENDERER_PLANE_H
