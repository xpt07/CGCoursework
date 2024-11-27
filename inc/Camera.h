#pragma once
#include "core.h"

class Camera {
public:
    vec3 position;
    vec3 target;
    vec3 up;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    Camera(vec3 pos, vec3 tgt, vec3 upDir, float fovAngle, float aspect, float near, float far)
        : position(pos), target(tgt), up(upDir), fov(fovAngle), aspectRatio(aspect), nearPlane(near), farPlane(far) {}

    Matrix getViewMatrix() {
        return Matrix::LookAt(position, target, up);
    }

    Matrix getProjectionMatrix() {
        return Matrix::Projection(fov, aspectRatio, nearPlane, farPlane);
    }

    void move(vec3 delta) {
        position += delta;
        target += delta;
    }

    void setPosition(vec3 newPos) {
        vec3 delta = newPos - position;
        move(delta);
    }

    void lookAt(vec3 newTarget) {
        target = newTarget;
    }

    void updateAspectRatio(float newAspectRatio) {
        aspectRatio = newAspectRatio;
    }
};
