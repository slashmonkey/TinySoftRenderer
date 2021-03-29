
#ifndef SOFTRENDERER_GEOMETRY_H
#define SOFTRENDERER_GEOMETRY_H

#include "Color.h"
#include "Math.h"

class Vertex{
public:
    Vertex(){ pos = Vec4f(); color = Vec3f(); normal = Vec3f(); texcoord = Vec2f(); }

    Vertex(Vec4f _pos) : pos(_pos) {}
    Vertex(Vec4f _pos, Color _color) :pos(_pos), color(_color) {}
    Vertex(Vec4f _pos, Color _color, Vec2f _texcoord) :pos(_pos), color(_color),texcoord(_texcoord) {}
    Vertex(Vec4f _pos, Vec3f _normal, Vec2f _texcoord) :pos(_pos), normal(_normal), texcoord(_texcoord) {}
    Vertex(Vec4f _pos, Color _color, Vec2f _texcoord, Vec3f _normal)
            :pos(_pos), color(_color), texcoord(_texcoord), normal(_normal) {}
    Vertex(const Vertex& rhs):pos(rhs.pos), color(rhs.color), texcoord(rhs.texcoord), normal(rhs.normal) {}

    void set_pos(Vec4f _pos) { pos = _pos; }
    void set_Normal(Vec3f n) { normal = n; }
    void set_TexCoord(float s, float t) { texcoord = Vec2f(s, t); }
    void set_Color(float r, float g, float b) { color = Color(r / 255.f, g / 255.f, b / 255.f); }

    float x() const { return pos.x; }
    float y() const { return pos.y; }
    float z() const { return pos.z; }
    float w() const { return pos.w; }

public:
    Vec4f pos;
    Vec3f normal;
    Vec2f texcoord;
    Color color;
};

class VertexOut{
public:
    //Reciprocal of the Homogeneous W
    float rhw = 1.f;
    Vec4f pos_world;    //世界变换后的坐标
    Vec4f pos_homo;     //投影变换后的坐标
    Color color;
    Vec3f normal;
    Vec2f texcoord;

    VertexOut() = default;
    //temp constructor
    explicit VertexOut(const Vec4f& _posHomo) : pos_homo(_posHomo){}
    VertexOut(const Vec4f& _posWorld, const Vec4f& _posHomo, const Vec2f& _tex, const Vec3f& _normal,
              const Color& _color, float _oneDivW)
            :pos_world(_posWorld), pos_homo(_posHomo),texcoord(_tex),
             normal(_normal), color(_color), rhw(_oneDivW) {}
    VertexOut(const VertexOut& rhs) :pos_world(rhs.pos_world),pos_homo(rhs.pos_homo),
                                     texcoord(rhs.texcoord), normal(rhs.normal),color(rhs.color),rhw(rhs.rhw) {}
};

class Triangle{
public:
    Triangle() {};
    Triangle(Vertex v0, Vertex v1, Vertex v2) :v0(v0), v1(v1), v2(v2) {}
    Triangle(const Triangle& t) { v0 = t.get_v0(); v1 = t.get_v1(); v2 = t.get_v2(); }
    Triangle& operator=(const Triangle& t) { v0 = t.get_v0(); v1 = t.get_v1(); v2 = t.get_v2(); return *this; }
    ~Triangle() {}

    void set_v0(const Vertex& A) { v0 = A; };
    Vertex get_v0() const { return v0; };

    void set_v1(const Vertex& B) { v1 = B; };
    Vertex get_v1() const { return v1; };

    void set_v2(const Vertex& C) { v2 = C; };
    Vertex get_v2() const { return v2; };
private:
    Vertex v0;
    Vertex v1;
    Vertex v2;
};

class TriangleOut{
public:
    TriangleOut() {};
    TriangleOut(VertexOut v0, VertexOut v1, VertexOut v2) :v0(v0), v1(v1), v2(v2) {}
    TriangleOut(const TriangleOut& t) { v0 = t.get_v0(); v1 = t.get_v1(); v2 = t.get_v2(); }
    TriangleOut& operator=(const TriangleOut& t) { v0 = t.get_v0(); v1 = t.get_v1(); v2 = t.get_v2(); return *this; }
    ~TriangleOut() {}

    void set_v0(const VertexOut& A) { v0 = A; };
    VertexOut get_v0() const { return v0; };

    void set_v1(const VertexOut& B) { v1 = B; };
    VertexOut get_v1() const { return v1; };

    void set_v2(const VertexOut& C) { v2 = C; };
    VertexOut get_v2() const { return v2; };
private:
    VertexOut v0;
    VertexOut v1;
    VertexOut v2;
};

#endif //SOFTRENDERER_GEOMETRY_H
