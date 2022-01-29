
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "Texture2D.h"
#include "Dependencies/stb_image.h"

Texture2D::~Texture2D() {
    stbi_image_free(pixelBuffer);
    pixelBuffer = nullptr;
}

bool Texture2D::load(const std::string& file_path) {
    if (pixelBuffer) delete pixelBuffer;
    pixelBuffer = nullptr;
    pixelBuffer = stbi_load(file_path.c_str(), &width, &height, &channel, 0);
    if(pixelBuffer == nullptr)
    {
        std::cout << "Failed to load image: " << file_path << std::endl;
    }
    return  pixelBuffer != nullptr;
}

Color Texture2D::sample_nearest(const Vec2f& texcoord) const {
    auto u_img = texcoord.x * (width - 1);
    auto v_img = texcoord.y * (height - 1);

    unsigned int x = 0, y = 0;
    x = static_cast<unsigned int>(u_img);
    y = static_cast<unsigned int>(v_img);

    int index[1];
    index[0] = (x * width + y) * channel;

    Color texel (static_cast<float>(pixelBuffer[index[0] + 0]), static_cast<float>(pixelBuffer[index[0] + 1]), static_cast<float>(pixelBuffer[index[0] + 2]));
    return texel;
}
