
#ifndef SOFTRENDERER_CAMERA_H
#define SOFTRENDERER_CAMERA_H

#include "Math.h"

//OpenGL handed system
//Eye coordinates are defined in the right-handed coordinate system, but NDC uses the left-handed coordinate system.
class Camera {
public:
    Camera(Vec3f eye, Vec3f lookAtTarget, Vec3f upDir): position(eye), target(lookAtTarget), up(upDir){
        look_at(eye, lookAtTarget, upDir);
    }

public:
    void set_projection(float l, float r, float b, float t, float n, float f);
    void set_projection(float fov, float ratio, float n, float f);
    void set_ortho(float l, float r, float b, float t, float n, float f);
    void look_at(Vec3f& from, Vec3f& to, Vec3f& upDir);

    Mat4f get_view_matrix() const{ return view; }
    Mat4f get_projection_matrix() const{ return projection; }

private:
    Mat4f view;
    Mat4f projection;

private:
    Vec3f position;
    Vec3f target;
    Vec3f up;

    float near = 1.0f;
    float far = 500.0f;
    float fov = TO_RADIANS(90);
};


#endif //SOFTRENDERER_CAMERA_H
