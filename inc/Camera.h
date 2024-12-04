#pragma once
#include "Window.h"
#include "core.h"

class Camera {
public:
    vec3 position;          
    Quaternion rotation;    
    float moveSpeed = 0.01f;
    float lookSensitivity = 0.001f;
    float pitch = 0.0f;
    float yaw = 0.0f;  

    Camera(vec3 startPos = vec3(0, 0, 0)) : position(startPos), rotation(Quaternion()) {}

    void update(Window& window) {

        POINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(window.hwnd, &cursorPos);

        vec2 center(window.width / 2, window.height / 2);
        yaw += (cursorPos.x - center.x) * lookSensitivity;
        pitch += (cursorPos.y - center.y) * lookSensitivity;

        pitch = max(-M_PI / 2.0f, min(M_PI / 2.0f, pitch));

        window.centerCursor();

        Quaternion yawRot(cos(yaw / 2), 0, sin(yaw / 2), 0);
        Quaternion pitchRot(cos(pitch / 2), sin(pitch / 2), 0, 0);
        rotation = (yawRot * pitchRot);

        vec3 forward = rotation.toMatrix().mulVec(vec3(0, 0, -1)).normalize();
        vec3 right = rotation.toMatrix().mulVec(vec3(1, 0, 0)).normalize();
        
        vec3 moveDir(0, 0, 0);
        if (window.keys['W']) moveDir += forward;
        if (window.keys['S']) moveDir -= forward;
        if (window.keys['A']) moveDir -= right;
        if (window.keys['D']) moveDir += right;

        if (moveDir.getLength() > 0) {
            position += moveDir.normalize() * moveSpeed;
        }
    }

    Matrix getViewMatrix() {
        return rotation.toMatrix().mul(Matrix::translation(-position));
    }
};
