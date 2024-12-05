#include "../inc/Window.h"
#include "../inc/Geometry.h"
#include "../inc/Timer.h"
#include "../inc/Camera.h"
#include "../inc/core.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    DXCore dx;
    Window win;
    ShaderManager shaderManager;
    Timer timer;
    Camera camera(vec3(vec3(0, 2, -20)));

    TextureManager textureManager;

    win.init(1024, 1024, "CGCoursework");
    dx.init(win.width, win.height, win.hwnd, false);

    ShowCursor(FALSE);

    Model trex;
    trex.init("resources/TRex.gem", dx, ModelType::ANIMATED);

    Plane plane;
    plane.init(dx);

    //Sphere sphere;
    //sphere.init(20, 20, 2.0f, dx);

    textureManager.load(dx, "resources/Textures/T-rex_Base_Color.png");

    shaderManager.loadShader("shaderAnim", "VShaderAnim.hlsl", "TexPixelShader.hlsl", dx);
    shaderManager.loadShader("shaderStat", "VertexShader.hlsl", "PixelShader.hlsl", dx);

    AnimationInstance trexAnimInstance;
    trexAnimInstance.animation = &trex.animation;
    trexAnimInstance.resetAnimationTime();

    Matrix worldMatrix = Matrix();

    while (true) {
        float dt = timer.update();

        camera.update(win, dt);
        Matrix view = camera.getViewMatrix();
        Matrix projection = projection.Projection(M_PI / 4.0f, float(win.width) / win.height, 0.1f, 100.0f);
        Matrix VP = projection.mul(view);

        dx.clear();

        // Draw Plane
        worldMatrix = worldMatrix.translation(vec3(0, -1, 0));
        shaderManager.getShader("shaderStat")->updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        shaderManager.applyShader("shaderStat", dx);
        plane.geometry.draw(dx);

        // Draw Animated Trex
        trexAnimInstance.update("Run", dt);
        worldMatrix = worldMatrix.translation(vec3(4, -1, 0));
        shaderManager.getShader("shaderAnim")->updateConstantVS("animatedMeshBuffer", "W", &worldMatrix);
        shaderManager.getShader("shaderAnim")->updateConstantVS("animatedMeshBuffer", "bones", trexAnimInstance.matrices);
        shaderManager.getShader("shaderAnim")->updateTexturePS("tex", textureManager.find("resources/Textures/T-rex_Base_Color.png"), dx);
        shaderManager.applyShader("shaderAnim", dx);
        trex.draw(dx);

        //// Draw Sphere
        //Matrix sphereWorld = Matrix::translation(vec3(-3, 1, 0)); // Offset to the left
        //shader.updateConstantVS("staticMeshBuffer", "W", &sphereWorld);
        //shader.apply(dx);
        //sphere.geometry.draw(dx);

        shaderManager.getShader("shaderStat")->updateConstantVS("staticMeshBuffer", "VP", &VP);
        shaderManager.getShader("shaderAnim")->updateConstantVS("animatedMeshBuffer", "VP", &VP);

        win.processMessages();
        dx.present();
    }
}
