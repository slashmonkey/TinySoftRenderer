
#ifndef SOFTRENDERER_LIGHT_H
#define SOFTRENDERER_LIGHT_H

#include "Math.h"
#include "Color.h"

struct Ambient {
public:
    Ambient(const Color& _color, double _intensity) : color(_color), intensity(_intensity){}
    ~Ambient() = default;

    Color get_color() const { return color; };
    double get_intensity() const { return intensity; }
private:
    Color color;
    double intensity;
};

struct Light{
public:
    Light(const Vec3f& _pos, const Color& _color, float _intensity) : position(_pos), color(_color), intensity(_intensity){};
    virtual ~Light() = default;

    Vec3f get_position() const { return position; }
    Color get_color() const { return color; }
    float get_intensity() const { return intensity; }
private:
    Vec3f position;
    Color color;
    float intensity;
};

struct DirectionalLight : public Light{
public:
    DirectionalLight(const Vec3f& _pos, const Color& _color, float _intensity): Light(_pos, _color, _intensity){}
    ~DirectionalLight() override = default;
};

struct Material{
public:
    Material(double _diffuse, double _specular, double _shininess) : diffuse(_diffuse) , specular(_specular), shininess(_shininess){}
    ~Material() = default;

    double get_diffuse() const { return diffuse; }
    double get_specular() const { return specular; }
    double get_shininess() const { return shininess; }
private:
    double diffuse;
    double specular;
    double shininess;
};
#endif //SOFTRENDERER_LIGHT_H
