#pragma once
#include "core.h"
#include "Window.h"
#include <Windows.h>

class Camera {
public:
    vec3 position;
    vec3 forward;
    vec3 up;
    vec3 right;
    float speed;
    float sensitivity;

    Camera(vec3 startPosition = vec3(0, 0, 0), vec3 startForward = vec3(0, 0, 1), float movementSpeed = 5.0f, float rotationSensitivity = 0.001f)
        : position(startPosition), forward(startForward.normalize()), speed(movementSpeed), sensitivity(rotationSensitivity) {
        up = vec3(0, 1, 0);
        right = forward.cross(up).normalize();
    }

    void update(Window& win, float deltaTime) {
        vec3 moveDirection(0, 0, 0);
        if (win.keys['W']) moveDirection += forward;
        if (win.keys['S']) moveDirection -= forward;
        if (win.keys['A']) moveDirection -= right;
        if (win.keys['D']) moveDirection += right;

        if (moveDirection.getLength() > 0.0f)
            moveDirection = moveDirection.normalize() * speed * deltaTime;

        position += moveDirection;

        handleMouseInput(win);
    }

    Matrix getViewMatrix() const {
        vec3 target = position + forward;
        return Matrix::LookAt(position, target, up);
    }

private:
    void handleMouseInput(Window& win) {
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(win.hwnd, &cursorPos);

        // Calculate the delta from the window's center
        int centerX = win.width / 2;
        int centerY = win.height / 2;
        int deltaX = cursorPos.x - centerX;
        int deltaY = cursorPos.y - centerY;

        // Fix for inverted left/right movement
        rotateYaw(-deltaX * sensitivity); // Invert deltaX
        rotatePitch(-deltaY * sensitivity);

        // Recenter the cursor
        win.centerCursor();
    }

    void rotateYaw(float angle) {
        Quaternion yaw = Quaternion::fromAxisAngle(up, angle);
        forward = yaw * forward;
        right = yaw * right;
    }

    void rotatePitch(float angle) {
        Quaternion pitch = Quaternion::fromAxisAngle(right, angle);
        vec3 newForward = pitch * forward;

        // Prevent camera from flipping
        if (abs(newForward.dot(up)) < 0.99f) {
            forward = newForward;
            up = right.cross(forward).normalize();
        }
    }
};
