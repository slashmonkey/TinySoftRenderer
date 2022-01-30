
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

Color Texture2D::sample_bilinear(const Vec2f& texcoord) const {
    Color result;
    if(pixelBuffer == nullptr) return result;

    //tex_x, tex_y on texture
    unsigned int tex_x = 0, tex_y = 0;
    float sample_x = 0.f;
    float sample_y = 0.f;
    // for bilinear interpolation.
    double factorU = 0, factorV = 0;

    if(texcoord.x >= 0.0f && texcoord.x <= 1.0f && texcoord.y >= 0.0f && texcoord.y <= 1.0f)
    {
        sample_x = texcoord.x * (float)(width - 1);
        sample_y = texcoord.y * (float)(height - 1);
        tex_x = static_cast<unsigned int>(sample_x);
        tex_y = static_cast<unsigned int>(sample_y);
        factorU = sample_x - tex_x;
        factorV = sample_y - tex_y;
    }else{
        // repeating.
        float u_clamp = texcoord.x, v_camp = texcoord.y;
        if(texcoord.x > 1.0f)
            u_clamp = texcoord.x - static_cast<int>(texcoord.x);
        else if(texcoord.x < 0.0f)
            u_clamp = 1.0f - (static_cast<int>(texcoord.x) - texcoord.x);
        if(texcoord.y > 1.0f)
            v_camp = texcoord.y - static_cast<int>(texcoord.y);
        else if(texcoord.y < 0.0f)
            v_camp = 1.0f - (static_cast<int>(texcoord.y) - texcoord.y);

        sample_x = u_clamp * (width - 1);
        sample_y = v_camp * (height - 1);
        tex_x = static_cast<unsigned int>(sample_x);
        tex_y = static_cast<unsigned int>(sample_y);
        factorU = sample_x - tex_x;
        factorV = sample_y - tex_y;
    }

    /*
     * u01, u11
     * u00, u10
     * */
    int u00 = tex_x * width + tex_y;
    int u01 = 0;
    int u11 = 0;
    int u10 = 0;

    if ( (tex_y + 1) >= height ){ u01 = u00; } else{ u01 = tex_x * width + tex_y + 1; }
    if ( (tex_x + 1) >= width ) { u10 = u00; } else{ u10 = (tex_x + 1) * width + tex_y; }
    if ((tex_y + 1) >= height || (tex_x + 1) >= width) { u11 = u00; }else{ u11 = (tex_x + 1) * width + tex_y + 1; }

    Color texels[4] {
        Color(static_cast<float>(pixelBuffer[u00 * channel + 0]), static_cast<float>(pixelBuffer[u00 * channel + 1]), static_cast<float>(pixelBuffer[u00 * channel + 2])),
        Color(static_cast<float>(pixelBuffer[u01 * channel + 0]), static_cast<float>(pixelBuffer[u01 * channel + 1]), static_cast<float>(pixelBuffer[u01 * channel + 2])),
        Color(static_cast<float>(pixelBuffer[u11 * channel + 0]), static_cast<float>(pixelBuffer[u11 * channel + 1]), static_cast<float>(pixelBuffer[u11 * channel + 2])),
        Color(static_cast<float>(pixelBuffer[u10 * channel + 0]), static_cast<float>(pixelBuffer[u10 * channel + 1]), static_cast<float>(pixelBuffer[u10 * channel + 2]))
    };

    // bilinear interpolation
    //horizontal
    texels[0] = texels[0] * (1.0 - factorU) + texels[3] * factorU;
    texels[1] = texels[1] * (1.0 - factorU) + texels[2] * factorU;
    //vertical
    result = texels[0] * (1.0 - factorV) + texels[1] *factorV;

    return result;
}
