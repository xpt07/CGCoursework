#include "../inc/Window.h"
#include "../inc/Geometry.h"
#include "../inc/Shaders.h"
#include "../inc/Timer.h"
#include "../inc/core.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    DXCore dx;
    Window win;
    Shaders shader;
    Timer timer;

    win.init(1024, 1024, "3D Geometry");
    dx.init(win.width, win.height, win.hwnd, false);

    Plane plane;
    plane.init(dx);

    Cube cube;
    cube.init(dx);

    Sphere sphere;
    sphere.init(20, 20, 2.0f, dx);

    shader.init("VertexShader.hlsl", "PixelShader.hlsl", dx);

    Matrix worldMatrix = Matrix();

    while (true) {
        float t = timer.elapsed();

        vec3 from = vec3(11 * cos(t), 5, 11 * sinf(t)); // Update camera position
        Matrix view = view.LookAt(from, vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        Matrix projection = projection.Projection(M_PI / 4.0f, float(win.width) / win.height, 0.1f, 100.0f);
        Matrix VP = projection.mul(view);

        dx.clear();

        // Draw Plane
        Matrix planeWorld = Matrix::translation(vec3(0, -1, 0)); // Place the plane below the cubes
        shader.updateConstantVS("staticMeshBuffer", "W", &planeWorld);

        shader.apply(dx);
        plane.geometry.draw(dx);

        // Draw First Cube
        Matrix cube1World = Matrix::translation(vec3(0, 1, 0)); // Centered at origin
        shader.updateConstantVS("staticMeshBuffer", "W", &cube1World);
        shader.apply(dx);
        cube.geometry.draw(dx);

        // Draw Second Cube
        Matrix cube2World = Matrix::translation(vec3(3, 1, 0)); // Offset to the right
        shader.updateConstantVS("staticMeshBuffer", "W", &cube2World);
        shader.apply(dx);
        cube.geometry.draw(dx);

        // Draw Sphere
        Matrix sphereWorld = Matrix::translation(vec3(-3, 1, 0)); // Offset to the left
        shader.updateConstantVS("staticMeshBuffer", "W", &sphereWorld);
        shader.apply(dx);
        sphere.geometry.draw(dx);

        shader.updateConstantVS("staticMeshBuffer", "VP", &VP);

        win.processMessages();
        dx.present();
    }
}
