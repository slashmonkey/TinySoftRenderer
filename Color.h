
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

    template<class T>
    Color operator* (const T t) const { return Color(this->R() * t, this->G() * t, this->B() * t); }
    Color operator+ (const Color& color) const {
        return Color(this->R() + color.R(), this->G() + color.G(), this->B() + color.B());
    }

    Color lerp(const Color& c, float weight) const{
        Color color;
        color.r = (*this).R() * (1 - weight) + c.R() * weight;
        color.g = (*this).G() * (1 - weight) + c.G() * weight;
        color.b = (*this).B() * (1 - weight) + c.B() * weight;
        return color;
    }

private:
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
};

const Color Black = Color(0.f, 0.f, 0.f);
const Color White = Color(255.f, 255.f, 255.f);
const Color Red   = Color(255.f, 0.f, 0.f);
const Color Green = Color(0.f, 255.f, 0.f);
const Color Blue  = Color(0.f, 0.f, 255.f);


#endif //SOFTRENDERER_COLOR_H
