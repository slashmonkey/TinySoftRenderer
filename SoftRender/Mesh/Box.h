
#ifndef SOFTRENDERER_BOX_H
#define SOFTRENDERER_BOX_H

#include "Mesh.h"

class Box : public Mesh {
public:
    Box(std::shared_ptr<IShader>& iShader);
    Box(float width, float height, float depth, std::shared_ptr<IShader>& iShader);

    ~Box() = default;

    void update_transform(float deltaTime) override;

private:
    float angle = 0.f;
};


#endif //SOFTRENDERER_BOX_H
