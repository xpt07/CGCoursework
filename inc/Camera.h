#pragma once
#include "Window.h"
#include "core.h"

class Camera {
public:
    vec3 position;
    Quaternion rotation;
    float moveSpeed = 5.0f;
    float lookSensitivity = 0.1f;

    Camera(vec3 startPos = vec3(0, 0, 0)) : position(startPos), rotation(Quaternion()) {}

    // Update the camera based on input
    void update(Window& window, float deltaTime) {
        vec3 forward = rotation.toMatrix().mulVec(vec3(0, 0, -1)).normalize();
        vec3 right = rotation.toMatrix().mulVec(vec3(1, 0, 0)).normalize();
        vec3 up = rotation.toMatrix().mulVec(vec3(0, 1, 0)).normalize();

        // Movement
        if (window.keys['W']) position += forward * moveSpeed * deltaTime;
        if (window.keys['S']) position -= forward * moveSpeed * deltaTime;
        if (window.keys['A']) position -= right * moveSpeed * deltaTime;
        if (window.keys['D']) position += right * moveSpeed * deltaTime;

        // Mouse look
        float yaw = window.mousex * lookSensitivity * deltaTime;
        float pitch = window.mousey * lookSensitivity * deltaTime;

        Quaternion yawRotation = Quaternion(cos(yaw / 2), 0, sin(yaw / 2), 0);
        Quaternion pitchRotation = Quaternion(cos(pitch / 2), sin(pitch / 2), 0, 0);

        rotation = (yawRotation * rotation * pitchRotation).normalize();
    }

    // Generate the view matrix
    Matrix getViewMatrix() {
        Matrix rotationMatrix = rotation.toMatrix();
        Matrix translationMatrix = Matrix::translation(-position);
        return rotationMatrix.mul(translationMatrix);
    }
};