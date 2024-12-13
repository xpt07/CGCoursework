#include "../inc/Camera.h"

void Camera::update(Window& win, float deltaTime) 
{
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

    // Normalize and scale the movement direction if applicable.
    if (moveDirection.getLength() > 0.0f) {
        moveDirection = moveDirection.normalize() * speed * deltaTime;
        position += moveDirection;
    }

    // Apply a bouncing effect when moving.
    if (isMoving) {
        timeAccumulator += deltaTime * bounceFrequency; // Increment time for the sine wave.
        position.y = baseY + sinf(timeAccumulator) * bounceAmplitude;
    }
    else {
        timeAccumulator = 0.0f;     // Reset bounce if not moving
        position.y = baseY;         // Reset to base height.
    }

    handleMouseInput(win);  // Process mouse input for rotation.
    realignUpVector();      // Ensure orientation vectors remain orthogonal.
}

Matrix Camera::getViewMatrix() const
{
    vec3 target = position + forward;               // Target point the camera is looking at.
    return Matrix::LookAt(position, target, up);    // Creates a view matrix using the LookAt method.
}

void Camera::handleMouseInput(Window& win) 
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);                   // Get the current cursor position.
    ScreenToClient(win.hwnd, &cursorPos);       // Convert to client coordinates.

    // Calculate the delta from the window's center
    int centerX = win.width / 2;
    int centerY = win.height / 2;
    int deltaX = cursorPos.x - centerX;
    int deltaY = cursorPos.y - centerY;

    // Rotate the camera based on the cursor's displacement.
    rotateYaw(-deltaX * sensitivity);       // Horizontal movement controls yaw.
    rotatePitch(-deltaY * sensitivity);     // Vertical movement controls pitch.

    // Recenter the cursor
    win.centerCursor();
}

void Camera::rotateYaw(float angle) 
{
    Quaternion yaw = Quaternion::fromAxisAngle(vec3(0, 1, 0), angle);   // Create a yaw rotation.
    forward = yaw * forward;    // Apply the rotation to the forward vector.
    right = yaw * right;        // Recalculate the right vector.
}

void Camera::rotatePitch(float angle)
{
    Quaternion pitch = Quaternion::fromAxisAngle(right, angle); // Create a pitch rotation.
    vec3 newForward = pitch * forward;  // Apply the rotation to the forward vector.

    // Prevent the camera from flipping over (limit vertical rotation).
    if (abs(newForward.dot(vec3(0, 1, 0))) < 0.99f) {
        forward = newForward;
    }
}

void Camera::realignUpVector()
{
    right = forward.cross(vec3(0, 1, 0)).normalize(); // Recalculate right vector.
    up = right.cross(forward).normalize();           // Recalculate up vector.
}
