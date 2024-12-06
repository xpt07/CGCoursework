#pragma once
#include "core.h"
#include "Window.h"
#include <Windows.h>
#include <cmath>

class Camera {
public:
    vec3 position;
    vec3 forward;
    vec3 up;
    vec3 right;
    float speed;
    float sensitivity;

    // Walking simulation variables
    float bounceAmplitude = 0.1f;   // Height of the bounce
    float bounceFrequency = 10.0f;  // Speed of the bounce
    float baseY;                   // Fixed y-level of the camera
    float timeAccumulator = 0.0f;  // Time accumulator for sine wave

    Camera(vec3 startPosition = vec3(0, 0, 0), vec3 startForward = vec3(0, 0, 1), float movementSpeed = 5.0f, float rotationSensitivity = 0.001f)
        : position(startPosition), forward(startForward.normalize()), speed(movementSpeed), sensitivity(rotationSensitivity), up(vec3(0,1,0)), 
        right(forward.cross(up).normalize()), baseY(startPosition.y)
    {}

    void update(Window& win, float deltaTime) {
        vec3 moveDirection(0, 0, 0);
        bool isMoving = false;

        if (win.keys['W']) {
            moveDirection += forward;
            isMoving = true;
        }
        if (win.keys['S']) {
            moveDirection -= forward;
            isMoving = true;
        }
        if (win.keys['A']) {
            moveDirection -= right;
            isMoving = true;
        }
        if (win.keys['D']) {
            moveDirection += right;
            isMoving = true;
        }

        if (moveDirection.getLength() > 0.0f) {
            moveDirection = moveDirection.normalize() * speed * deltaTime;
            position += moveDirection;
        }

        // Fix the y-level of the camera and apply bounce if moving
        if (isMoving) {
            timeAccumulator += deltaTime * bounceFrequency; // Update bounce time
            position.y = baseY + sinf(timeAccumulator) * bounceAmplitude;
        }
        else {
            timeAccumulator = 0.0f; // Reset bounce if not moving
            position.y = baseY;
        }

        handleMouseInput(win);
        realignUpVector();
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
        rotateYaw(-deltaX * sensitivity);
        rotatePitch(-deltaY * sensitivity);

        // Recenter the cursor
        win.centerCursor();
    }

    void rotateYaw(float angle) {
        Quaternion yaw = Quaternion::fromAxisAngle(vec3(0, 1, 0), angle);
        forward = yaw * forward;
        right = yaw * right;
    }

    void rotatePitch(float angle) {
        Quaternion pitch = Quaternion::fromAxisAngle(right, angle);
        vec3 newForward = pitch * forward;

        // Prevent camera from flipping
        if (abs(newForward.dot(vec3(0, 1, 0))) < 0.99f) {
            forward = newForward;
        }
    }

    void realignUpVector() {
        // Recalculate right vector to ensure it's perpendicular to the fixed world up
        right = forward.cross(vec3(0, 1, 0)).normalize();

        // Recalculate up to align with the fixed world up
        up = right.cross(forward).normalize();
    }
};
