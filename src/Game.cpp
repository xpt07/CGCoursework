#include "../inc/Window.h"
#include "../inc/Geometry.h"
#include "../inc/Shaders.h"
#include "../inc/core.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    DXCore dx;
    Window win;
    Plane plane;
    Shaders shader;

    win.init(1024, 1024, "3D Plane");
    dx.init(win.width, win.height, win.hwnd, false);
    plane.init(dx);
    shader.init("C:/Users/u5632744/Desktop/Computer Graphics/CGCoursework/VertexShader.hlsl", "C:/Users/u5632744/Desktop/Computer Graphics/CGCoursework/PixelShader.hlsl", dx);

    Matrix worldMatrix = Matrix();
    Matrix viewMatrix = Matrix::lookAt(vec3(0, 20, 30), vec3(0, 0, 0), vec3(0, 1, 0));
    Matrix projMatrix = Matrix::perspectiveFovLH(M_PI / 4.0f, 1024.0f / 768.0f, 0.1f, 100.0f);
    Matrix vpMatrix = viewMatrix.mul(projMatrix);

    while (true) {
        dx.clear();

        shader.updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        shader.updateConstantVS("staticMeshBuffer", "VP", &vpMatrix);

        shader.apply(dx);
        plane.geometry.draw(dx);

        win.processMessages();
        dx.present();
    }
}
