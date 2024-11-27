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

    shader.init("VertexShader.hlsl", "PixelShader.hlsl", dx);

    Matrix planeWorld = Matrix();

    while (true) {
        float t = timer.elapsed();

        vec3 from = vec3(11 * cos(t), 5, 11 * sinf(t)); // Update camera position
        Matrix view = view.LookAt(from, vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        Matrix projection = projection.Projection(M_PI / 4.0f, float(win.width) / win.height, 0.1f, 100.0f);
        Matrix VP = projection.mul(view);

        dx.clear();

        shader.updateConstantVS("staticMeshBuffer", "W", &planeWorld);
        shader.updateConstantVS("staticMeshBuffer", "VP", &VP);

        shader.apply(dx);
        cube.geometry.draw(dx);

        win.processMessages();
        dx.present();
    }
}
