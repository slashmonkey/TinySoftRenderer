
#ifndef SOFTRENDERER_OBJMODEL_H
#define SOFTRENDERER_OBJMODEL_H

#include "Mesh.h"

class ObjModel : public Mesh{
public:
    ObjModel(const std::string& file, std::shared_ptr<IShader>& iShader);
    ~ObjModel() = default;

    void update_transform(float deltaTime) override;
};


#endif //SOFTRENDERER_OBJMODEL_H
