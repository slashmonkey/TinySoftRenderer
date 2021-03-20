#ifndef SOFTRENDERER_MATH_H
#define SOFTRENDERER_MATH_H

#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>

const float EPSILON = 1e-5f;
const float PI = 3.1415926f;
const float DEG2RAD = PI / 180.0f;
const float RAD2DEG = 180.0f / PI;
const double tolerance = 1e-5f;

inline bool equal(double a, double b)
{
    return fabs(a - b) < tolerance;
}

#define TO_RADIANS(degrees) ((PI/180) * degrees);
#define TO_DEGREES(radians) ((180/PI) * radians);

template <typename T> struct Vector2{
    T x, y;
    Vector2<T>() : x(T()), y(T()) {}
    Vector2<T>(T _x, T _y): x(_x), y(_y){}
    Vector2<T>(const Vector2<T>& v): x(v.x), y(v.y){}
    ~Vector2() = default;
    Vector2<T>& operator =(const Vector2<T> &v){
        if(this != &v) { x = v.x; y = v.y;}
        return *this;
    }

    Vector2<T> operator +(const Vector2& v) const { return Vector2<T>(x + v.x, y + v.y);}
    Vector2<T> operator -(const Vector2& v) const { return Vector2<T>(x - v.x, y - v.y);}
    Vector2<T> operator *(const Vector2& v) const { return Vector2<T>(x * v.x, y * v.y);}
    Vector2<T> operator *(T f) const { return Vector2<T>(x*f, y*f);}

    void operator+=(const Vector2<T> &v){x+=v.x;    y+=v.y;}
    void operator-=(const Vector2<T> &v){x-=v.x;    y-=v.y;}
    void operator*=(const T f){x*=f; y*=f;}

    T operator[](const int i) const { if(i<=0) return x;else return y;}

    bool operator ==(const Vector2& v) const {return x == v.x && y == v.y;}
    bool operator !=(const Vector2& v) const {return x != v.x || y != v.y;}

    Vector2<T> lerp(const Vector2<T>& v2, float weight) const {return (*this) * (1 - weight) + v2 * weight;}

    template<typename> friend std::ostream& operator<<(std::ostream& s, Vector2<T>& v);
};

template <typename T> std::ostream& operator<<(std::ostream& s, Vector2<T>& v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template <typename T> struct Vector3{
    T x, y, z;
    Vector3<T>() : x(T()), y(T()), z(T()) {}
    Vector3<T>(T _x, T _y, T _z): x(_x), y(_y), z(_z){}
    Vector3<T>(const Vector3<T>& v): x(v.x), y(v.y), z(v.z){}
    ~Vector3() = default;
    Vector3<T>& operator =(const Vector3<T> &v){
        if(this != &v) { x = v.x; y = v.y; z = v.z;}
        return *this;
    }

    T norm() const{return std::sqrt(x*x + y*y + z*z);}
    Vector3<T>& normalize() { T norm = this->norm(); assert(norm != 0); norm = 1/norm; *this = *this * norm; return *this; }
    T dot(const Vector3<T> v) {return x * v.x + y*v.y + z*v.z;}
    Vector3<T> cross(const Vector3<T>& v) {
        T m1, m2, m3;
        m1 = y * v.z - z * v.y;
        m2 = z * v.x - x * v.z;
        m3 = x * v.y - y * v.x;
        return Vector3<T>(m1, m2, m3);
    }

    Vector3<T> operator +(const Vector3& v) const { return Vector3<T>(x + v.x, y + v.y, z + v.z);}
    Vector3<T> operator -(const Vector3& v) const { return Vector3<T>(x - v.x, y - v.y, z - v.z);}
    Vector3<T> operator *(const Vector3& v) const { return Vector3<T>(x * v.x, y * v.y, z * v.z);}
    Vector3<T> operator *(T f) const { return Vector3<T>(x*f, y*f, z*f);}
    T operator[](const int i) const { if(i<=0) return x;else if(i == 1) return y;else{return z;}}

    bool operator ==(const Vector3& v) const {return x == v.x && y == v.y && z == v.z;}
    bool operator !=(const Vector3& v) const {return x != v.x || y != v.y || z != v.z;}

    Vector3<T> lerp(const Vector3<T>& v2, float weight) const {return (*this) * (1 - weight) + v2 * weight;}

    template<typename> friend std::ostream& operator<<(std::ostream& s, Vector3<T>& v);
};

template <typename T> std::ostream& operator<<(std::ostream& s, Vector3<T>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}

template <typename T> struct Vector4{
    T x, y, z, w;
    Vector4<T>() : x(T()), y(T()), z(T()), w(static_cast<T>(1)) {}
    Vector4<T>(T _x, T _y, T _z, T _w): x(_x), y(_y), z(_z), w(_w){}
    Vector4<T>(const Vector4<T>& v): x(v.x), y(v.y), z(v.z), w(v.w){}
    Vector4<T>(const Vector3<T>& v): x(v.x), y(v.y), z(v.z), w(static_cast<T>(1)){}
    ~Vector4() = default;
    Vector4<T>& operator =(const Vector4<T> &v){
        if(this != &v) { x = v.x; y = v.y; z = v.z; w = v.w;}
        return *this;
    }

    T norm() const{return std::sqrt(x*x + y*y + z*z);}
    Vector4<T>& normalize() { T norm = this->norm(); assert(norm != 0); *this = *this * 1/norm; return *this; }
    Vector4<T> operator +(const Vector4& v) const { return Vector4<T>(x + v.x, y + v.y, z + v.z, w + v.w);}
    Vector4<T> operator -(const Vector4& v) const { return Vector4<T>(x - v.x, y - v.y, z - v.z, w - v,x);}
    Vector4<T> operator *(const Vector4& v) const { return Vector4<T>(x * v.x, y * v.y, z * v.z, w * v.w);}
    Vector4<T> operator *(T f) const { return Vector4<T>(x*f, y*f, z*f, w*f);}

    T operator[](const int i) const { if(i<=0) return x;else if(i == 1) return y;else if (i == 2) return z;else{return w;}}

    bool operator ==(const Vector4& v) const {return x == v.x && y == v.y && z == v.z && w == v.w;}
    bool operator !=(const Vector4& v) const {return !((*this) == v);}

    Vector4<T> lerp(const Vector4<T>& v2, float weight) const {return (*this) * (1 - weight) + v2 * weight;}

    template<typename> friend std::ostream& operator<<(std::ostream& s, Vector4<T>& v);
};

template <typename T> std::ostream& operator<<(std::ostream& s, Vector4<T>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z <<", " << v.w << ")\n";
    return s;
}

/*
    Matrix4x4 Column Major.
    Elements are layout in row-major style in two vectors.
    x1, x2, x3, t1,
    y1, y2, y3, t2,
    z1, z2, z3, t3,
     0,  0,  0,  1

     x1, x2, x3, t1,y1, y2, y3, t2,z1, z2, z3, t3,0,  0,  0,  1
*/
template <typename T>
struct Matrix4x4 {
    const static int rows = 4;
    const static int cols = 4;
    std::vector<std::vector<T>> m;

    Matrix4x4<T>() : m(std::vector<std::vector<T>>(rows, std::vector<T>(cols, 0.0))){}

    Matrix4x4<T>(const Matrix4x4<T>& rhs) : m(rhs.m){}

    Matrix4x4<T>(const T f) : m(identity().m){
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                m[i][j] *= f;
            }
        }
    }

    static Matrix4x4<T> identity(){
        Matrix4x4<T> out;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                out[i][j] = i == j? static_cast<T>(1):static_cast<T>(0);
            }
        }
        return out;
    }

    std::vector<T>& operator[](const int i) { assert(i >=0 && i < 4); return m[i];}
    std::vector<T> operator[](const int i) const { assert(i >=0 && i < 4); return m[i];}

    Matrix4x4<T>& operator=(const Matrix4x4<T>& m){
        if(this != &m){
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    this->m[i][j] = m[i][j];
                }
            }
        }
        return *this;
    }

    Matrix4x4<T> operator+(const Matrix4x4<T>& m) const{
        Matrix4x4<T> out;
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                out.m[i][j] = this->m[i][j] + m[i][j];
            }
        }
        return out;
    }

    Matrix4x4<T> operator-(const Matrix4x4<T>& m) const{
        Matrix4x4<T> out;
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                out.m[i][j] = this->m[i][j] - m[i][j];
            }
        }
        return out;
    }

    Matrix4x4<T> operator*(const Matrix4x4<T>& rhs) const{
        Matrix4x4<T> out;
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                for (int k = 0; k < rows; ++k) {
                    out.m[i][j] += m[i][k] * rhs[k][j];
                }
            }
        }
        return out;
    }

    Vector4<T> operator*(const Vector4<T>& vec4) const{
        T a = 0, b = 0, c = 0, d = 0;

        for (int j = 0; j < cols; ++j){
            a += this->m[0][j] * vec4[j];
        }

        for (int j = 0; j < cols; ++j){
            b += this->m[1][j] * vec4[j];
        }

        for (int j = 0; j < cols; ++j){
            c += this->m[2][j] * vec4[j];
        }

        for (int j = 0; j < cols; ++j){
            d += this->m[3][j] * vec4[j];
        }

        Vector4<T> out(a, b ,c, d);
        return out;
    }

    Matrix4x4<T> operator*(const T k) const{
        Matrix4x4<T> out;
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                out.m[i][j] = this->m[i][j] * k;
            }
        }
        return out;
    }

    void set_row(const Vector3<T>& vec3, int row){
        assert(row < rows);
        Vector4<T> vec4(vec3);
        set_row(vec4, row);
    }

    void set_row(const Vector4<T>& vec4, int row){
        assert(row < rows);
        for (int i = 0; i < cols ; ++i) {
            m[row][i] = vec4[i];
        }
    }

    void set_translation(const Vector3<T>& translation){
        *this = identity();
        m[0][3] = translation.x;
        m[1][3] = translation.y;
        m[2][3] = translation.z;
    }

    void set_scale(const Vector3<T>& scaleFactor){
        *this = identity();
        m[0][0] = scaleFactor.x;
        m[1][1] = scaleFactor.y;
        m[2][2] = scaleFactor.z;
    }

    void rotationAxis(const T angle, const Vector3<T>& axis){
        *this = identity();
    }

    void set_rotationX(const T angle){
        *this = identity();
        float c = std::cos(angle * DEG2RAD);
        float s = std::sin(angle * DEG2RAD);
        m[1][1] = c;
        m[1][2] = -s;
        m[2][1] = s;
        m[2][2] = c;
    }

    void set_rotationY(const T angle){
        *this = identity();
        float c = std::cos(angle * DEG2RAD);
        float s = std::sin(angle * DEG2RAD);
        m[0][0] = c;
        m[0][2] = s;
        m[2][0] = -s;
        m[2][2] = c;
    }

    void set_rotationZ(const T angle){
        *this = identity();
        float c = std::cos(angle * DEG2RAD);
        float s = std::sin(angle * DEG2RAD);
        m[0][0] = c;
        m[0][1] = -s;
        m[1][0] = s;
        m[1][1] = c;
    }

    Matrix4x4 inverse(){
        float SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
        float SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
        float SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
        float SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
        float SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
        float SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
        float SubFactor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
        float SubFactor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
        float SubFactor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
        float SubFactor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
        float SubFactor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
        float SubFactor11 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
        float SubFactor12 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
        float SubFactor13 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
        float SubFactor14 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
        float SubFactor15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
        float SubFactor16 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
        float SubFactor17 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

        Matrix4x4<float> adj;

        adj.m[0][0] = +(m[1][1] * SubFactor00 - m[1][2] * SubFactor01 + m[1][3] * SubFactor02);
        adj.m[0][1] = -(m[1][0] * SubFactor00 - m[1][2] * SubFactor03 + m[1][3] * SubFactor04);
        adj.m[0][2] = +(m[1][0] * SubFactor01 - m[1][1] * SubFactor03 + m[1][3] * SubFactor05);
        adj.m[0][3] = -(m[1][0] * SubFactor02 - m[1][1] * SubFactor04 + m[1][2] * SubFactor05);

        adj.m[1][0] = -(m[0][1] * SubFactor00 - m[0][2] * SubFactor01 + m[0][3] * SubFactor02);
        adj.m[1][1] = +(m[0][0] * SubFactor00 - m[0][2] * SubFactor03 + m[0][3] * SubFactor04);
        adj.m[1][2] = -(m[0][0] * SubFactor01 - m[0][1] * SubFactor03 + m[0][3] * SubFactor05);
        adj.m[1][3] = +(m[0][0] * SubFactor02 - m[0][1] * SubFactor04 + m[0][2] * SubFactor05);

        adj.m[2][0] = +(m[0][1] * SubFactor06 - m[0][2] * SubFactor07 + m[0][3] * SubFactor08);
        adj.m[2][1] = -(m[0][0] * SubFactor06 - m[0][2] * SubFactor09 + m[0][3] * SubFactor10);
        adj.m[2][2] = +(m[0][0] * SubFactor07 - m[0][1] * SubFactor09 + m[0][3] * SubFactor11);
        adj.m[2][3] = -(m[0][0] * SubFactor08 - m[0][1] * SubFactor10 + m[0][2] * SubFactor11);

        adj.m[3][0] = -(m[0][1] * SubFactor12 - m[0][2] * SubFactor13 + m[0][3] * SubFactor14);
        adj.m[3][1] = +(m[0][0] * SubFactor12 - m[0][2] * SubFactor15 + m[0][3] * SubFactor16);
        adj.m[3][2] = -(m[0][0] * SubFactor13 - m[0][1] * SubFactor15 + m[0][3] * SubFactor17);
        adj.m[3][3] = +(m[0][0] * SubFactor14 - m[0][1] * SubFactor16 + m[0][2] * SubFactor17);

        T Determinant =
                +m[0][0] * adj.m[0][0]
                + m[0][1] * adj.m[0][1]
                + m[0][2] * adj.m[0][2]
                + m[0][3] * adj.m[0][3];
        if(Determinant == 0) return identity();

        return adj / Determinant;
    }

    Matrix4x4<T> transpose(){
        Matrix4x4<T> out;
        for(int i=0; i<rows; i++)
            for(int j=0; j<cols; j++)
                out[j][i] = m[i][j];
        return out;
    }

    friend std::ostream& operator<<(std::ostream& s, Matrix4x4<T>& m){
        for (int i=0; i<m.rows; i++)  {
            for (int j=0; j<m.cols; j++) {
                s << m[i][j];
                if (j<m.rows-1) s << "\t";
            }
            s << "\n";
        }
        return s;
    }
};

typedef Vector2<float> Vec2f;
typedef Vector3<int> Vec3i;
typedef Vector3<float> Vec3f;
typedef Vector4<float> Vec4f;
typedef Matrix4x4<float> Mat4f;

inline int CLAMP(int x, int min, int max) { return (x < min) ? min : ((x > max) ? max : x); }
inline float CLAMP(float x, float min, float max) { return (x < min) ? min : ((x > max) ? max : x); }

#endif //SOFTRENDERER_MATH_H
