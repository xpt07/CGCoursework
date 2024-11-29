#include "../inc/Window.h"
#include "../inc/Geometry.h"
#include "../inc/Shaders.h"
#include "../inc/Timer.h"
#include "../inc/Camera.h"
#include "../inc/core.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    DXCore dx;
    Window win;
    Shaders shader;
    Timer timer;
    Camera camera(vec3(11, 5, 11));

    win.init(1024, 1024, "3D Geometry");
    dx.init(win.width, win.height, win.hwnd, false);

    //Model soldier;
    //soldier.init("C:/Users/u5632744/Desktop/Computer Graphics/CGCoursework/resources/Soldier/Models/Soldier1.gem", dx, true);

    //Model acaciaTree;
    //acaciaTree.init("C:/Users/u5632744/Desktop/Computer Graphics/CGCoursework/resources/Trees/Models/acacia.gem", dx, false);

    Model trex;
    trex.init("C:/Users/u5632744/Desktop/Computer Graphics/CGCoursework/resources/TRex.gem", dx, true);

    //Plane plane;
    //plane.init(dx);

    //Cube cube;
    //cube.init(dx);

    //Sphere sphere;
    //sphere.init(20, 20, 2.0f, dx);

    shader.init("VShaderAnim.hlsl", "PixelShader.hlsl", dx);

    AnimationInstance trexAnimInstance;
    trexAnimInstance.animation = &trex.animation;
    trexAnimInstance.resetAnimationTime();

    Matrix worldMatrix = Matrix();

    while (true) {
        float t = timer.elapsed();

        camera.update(win, t);
        Matrix view = camera.getViewMatrix();
        Matrix projection = projection.Projection(M_PI / 4.0f, float(win.width) / win.height, 0.1f, 100.0f);
        Matrix VP = projection.mul(view);

        dx.clear();

        // Draw Plane
        //worldMatrix = worldMatrix.translation(vec3(0, -1, 0)); // Place the plane below the cubes
        //shader.updateConstantVS("animatedMeshBuffer", "W", &worldMatrix);
        //shader.apply(dx);
        //plane.geometry.draw(dx);

        //trexAnimInstance.update("Run", t);
        worldMatrix = worldMatrix.scaling(vec3(0.5f, 0.5f, 0.5f)) * worldMatrix.translation(vec3(0, 1, 0));
        shader.updateConstantVS("animatedMeshBuffer", "W", &worldMatrix);
        shader.updateConstantVS("animatedMeshBuffer", "VP", &VP);
        shader.updateConstantVS("animatedMeshBuffer", "bones", trexAnimInstance.matrices);
        shader.apply(dx);
        trex.draw(dx);

        //worldMatrix = worldMatrix.scaling(vec3(0.01f, 0.01f, 0.01f)) * worldMatrix.translation(vec3(0, 1, 400));
        //shader.updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        //shader.apply(dx);
        //soldier.draw(dx);

        //worldMatrix = worldMatrix.scaling(vec3(0.01f, 0.01f, 0.01f)) * worldMatrix.translation(vec3(0, 1, 0));
        //shader.updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        //shader.apply(dx);
        //acaciaTree.draw(dx);

        //// Draw First Cube
        //worldMatrix = worldMatrix.scaling(vec3(0.1f, 0.1f, 0.1f)) * worldMatrix.translation(vec3(0, 50, 0));;
        ////cube1World = cube1World.translation(vec3(0, 1, 0)); // Centered at origin
        //shader.updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        //shader.apply(dx);
        //cube.geometry.draw(dx);

        //// Draw Second Cube
        //worldMatrix = worldMatrix.translation(vec3(3, 1, 0)); // Offset to the right
        //shader.updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        //shader.apply(dx);
        //cube.geometry.draw(dx);

        //// Draw Sphere
        //Matrix sphereWorld = Matrix::translation(vec3(-3, 1, 0)); // Offset to the left
        //shader.updateConstantVS("staticMeshBuffer", "W", &sphereWorld);
        //shader.apply(dx);
        //sphere.geometry.draw(dx);

        win.processMessages();
        dx.present();
    }
}
