
#ifndef SOFTRENDERER_TEXTURE2D_H
#define SOFTRENDERER_TEXTURE2D_H

#include <string>
#include "Math.h"
#include "Color.h"

class Texture2D {
public:
    Texture2D():width(0), height(0), channel(0), pixelBuffer(nullptr){};
    ~Texture2D();

    Color sample_nearest(const Vec2f & texcoord) const;
    bool load(const std::string& file_path);
private:
    int width;
    int height;
    int channel;
    unsigned char* pixelBuffer;
};


#endif //SOFTRENDERER_TEXTURE2D_H
