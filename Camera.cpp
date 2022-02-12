
#include "Camera.h"
#include <cmath>
#include <iostream>

void Camera::set_projection(float l, float r, float b, float t, float n, float f) {
    projection.set_row(Vec4f(2*n/(r-l),         0,  (r+l)/(r-l),            0), 0);
    projection.set_row(Vec4f(        0, 2*n/(t-b),  (t+b)/(t-b),            0), 1);
    projection.set_row(Vec4f(        0,         0, -(f+n)/(f-n), -2*f*n/(f-n)), 2);
    projection.set_row(Vec4f(        0,         0,           -1,            0), 3);
}

//n,f positive, fov in angle
void Camera::set_projection(float fov, float ratio, float n, float f) {
    //tan(fov/2) = t/|n| -> n/r
    //ratio = r/t        -> n/t
    float tan = std::tanf(fov * 0.5 * DEG2RAD);
    projection.set_row(Vec4f(1/(tan*ratio),          0,            0,            0), 0);
    projection.set_row(Vec4f(          0,      1/tan,            0,            0), 1);
    projection.set_row(Vec4f(          0,          0, -(f+n)/(f-n), -2*f*n/(f-n)), 2);
    projection.set_row(Vec4f(          0,          0,           -1,            0), 3);

    this->fov = fov;
    this->near = n;
    this->far = f;
}

void Camera::set_projection(float fov, float width, float height, float n, float f) {
    this->width = width;
    this->height = height;
    set_projection(fov, width/height, n, f);
}

void Camera::look_at(Vec3f& from, Vec3f& to, Vec3f& upDir) {
    position = from;
    target = to;

    //from - to, eye - target
    forward = (position - target).normalize();
    left = upDir.cross(forward).normalize();
    up = forward.cross(left).normalize();

    //Mview = Mr * Mt
    Mat4f mat4f = Mat4f::identity();

    Mat4f mr = Mat4f::identity();
    mr.set_row(Vec4f(left.x, left.y, left.z, 0.f), 0);
    mr.set_row(Vec4f(up.x, up.y, up.z, 0.f), 1);
    mr.set_row(Vec4f(forward.x, forward.y, forward.z, 0.f), 2);

    Mat4f mt = Mat4f::identity();
    mt.set_translation(Vec3f(-from.x,-from.y, -from.z));

    view = mr * mt;
}

//(-f, -n)->(1, -1), left handed in NDC
void Camera::set_ortho(float l, float r, float b, float t, float n, float f) {
    projection.set_row(Vec4f(2/(r-l),       0,        0, -1*(r+l)/(r-l)), 0);
    projection.set_row(Vec4f(      0, 2/(t-b),        0, -1*(t+b)/(t-b)), 1);
    projection.set_row(Vec4f(      0,       0, -2/(f-n), -1*(f+n)/(f-n)), 2);
    projection.set_row(Vec4f(      0,       0,        0,              1), 3);
}

void Camera::process_key_input(SDL_Keycode keyCode, float deltaTime) {
    switch (keyCode) {
        case SDLK_w:
            translate(forward * -speed * deltaTime);
            break;
        case SDLK_s:
            translate(forward * speed * deltaTime);
            break;
        case SDLK_a:
            translate(left * -speed * deltaTime);
            break;
        case SDLK_d:
            translate(left * speed * deltaTime);
            break;
        default:
            break;
    }
}

void Camera::translate(const Vec3f& trans) {
    position = position + trans;
//    target = target + trans;
    look_at(position, target, up);
}

void Camera::process_mouse_input(int x, int y, bool leftMouseDown, bool rightMouseDown, float delta_time) {
    if (first_init){
        last_mouse_x = x;
        last_mouse_y = y;
        first_init = false;
    }

    float x_delta = (x - last_mouse_x) / width;
    float y_delta = (y - last_mouse_y) / height;
    last_mouse_x = x;
    last_mouse_y = y;

    if (leftMouseDown){
        Vec3f from_target = position - target;			// vector point from target to camera's position
        float radius = from_target.norm();

        float phi     = (float)atan2(from_target[0], from_target[2]); // angle between from_target and z-axis[-pi, pi]
        float theta   = (float)acos(from_target[1] / radius); //angle between from_target and y-axis, [0, pi]

        float factor = 1.5f;
        phi   -= x_delta * factor;
        theta -= y_delta * factor;

        if (theta > PI) theta = PI - EPSILON * 100;
        if (theta < 0)  theta = EPSILON * 100;

        position.x = target.x + radius * sin(phi) * sin(theta);
        position.y = target.y + radius * cos(theta);
        position.z = target.z + radius * sin(theta) * cos(phi);

        look_at(position, target, up);
    }
}

