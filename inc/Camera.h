#pragma once
#include "core.h"
#include "Window.h"
#include <Windows.h>
#include <cmath>

// Camera class for controlling the player's perspective in a 3D environment.
class Camera {
public:
    vec3 position;          // Current position of the camera in the world.
    vec3 forward;           // Forward direction the camera is looking at.
    vec3 up;                // Up vector to define the camera's orientation.
    vec3 right;             // Right vector, perpendicular to forward and up.
    float speed;            // Movement speed of the camera.
    float sensitivity;      // Sensitivity for mouse input affecting rotation.

    // Walking simulation variables
    float bounceAmplitude = 0.1f;   // Height of the bounce
    float bounceFrequency = 10.0f;  // Speed of the bounce
    float baseY;                   // Fixed y-level of the camera
    float timeAccumulator = 0.0f;  // Time accumulator for sine wave

    // Constructor initializes the camera with position, direction, speed, and sensitivity.
    Camera(vec3 startPosition = vec3(0, 0, 0), vec3 startForward = vec3(0, 0, 1), float movementSpeed = 5.0f, float rotationSensitivity = 0.001f)
        : position(startPosition), forward(startForward.normalize()), speed(movementSpeed), sensitivity(rotationSensitivity), up(vec3(0,1,0)), 
        right(forward.cross(up).normalize()), baseY(startPosition.y)
    {}

    // Updates the camera position and orientation based on user input.
    void update(Window& win, float deltaTime);

    // Calculates and returns the view matrix for rendering the scene.
    Matrix getViewMatrix() const;

private:
    // Processes mouse input to rotate the camera's orientation.
    void handleMouseInput(Window& win);

    // Rotates the camera around the Y-axis (yaw).
    void rotateYaw(float angle);

    // Rotates the camera around the right vector (pitch).
    void rotatePitch(float angle);

    // Realigns the up vector to maintain proper orientation.
    void realignUpVector();
};
