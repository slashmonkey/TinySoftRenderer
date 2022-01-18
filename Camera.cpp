
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
    mr.set_row(Vec3f(left.x, left.y, left.z), 0);
    mr.set_row(Vec3f(up.x, up.y, up.z), 1);
    mr.set_row(Vec3f(forward.x, forward.y, forward.z), 2);

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
    target = target + trans;
    look_at(position, target, up);
}

