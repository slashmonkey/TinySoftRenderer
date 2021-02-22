
#ifndef SOFTRENDERER_COLOR_H
#define SOFTRENDERER_COLOR_H

#include "Math.h"

struct Color{
public:
    Color() : r(0), g(0), b(0) {};

    Color(const Vec3f& v3f) {
        r = CLAMP(v3f.x, 0.f ,255.f);
        g = CLAMP(v3f.y, 0.f ,255.f);
        b = CLAMP(v3f.z, 0.f ,255.f);
    }
    Color(float _r, float _g, float _b) {
        r = CLAMP(_r, 0.f, 255.f);
        g = CLAMP(_g, 0.f, 255.f);
        b = CLAMP(_b, 0.f, 255.f);
    };

    Color(const Color& c){
        r = c.r;
        g = c.g;
        b = c.b;
    };

    float R() const { return r; }
    float G() const { return g; }
    float B() const { return b; }

private:
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
};

const Color BLACK = Color(0.f, 0.f, 0.f);
const Color WHITE = Color(255.f, 255.f, 255.f);
const Color RED   = Color(255.f, 0.f, 0.f);


#endif //SOFTRENDERER_COLOR_H
