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

    void update(Window& win, float deltaTime);
    Matrix getViewMatrix() const;

private:
    void handleMouseInput(Window& win);
    void rotateYaw(float angle);
    void rotatePitch(float angle);
    void realignUpVector();
};
