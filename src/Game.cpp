#include "../inc/Window.h"
#include "../inc/Geometry.h"
#include "../inc/Shaders.h"
#include "../inc/Timer.h"
#include "../inc/Camera.h"
#include "../inc/core.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    DXCore dx;
    Window win;
    Shaders shaderAnim;
    Shaders shaderStat;
    Timer timer;
    Camera camera(vec3(vec3(0, 2, 10)));

    win.init(1024, 1024, "CGCoursework");
    dx.init(win.width, win.height, win.hwnd, false);

    ShowCursor(FALSE);

    //Model soldier;
    //soldier.init("C:/Users/u5632744/Desktop/Computer Graphics/CGCoursework/resources/Soldier/Models/Soldier1.gem", dx, true);

    Model acaciaTree;
    acaciaTree.init("C:/Users/u5632744/Desktop/Computer Graphics/CGCoursework/resources/Trees/Models/acacia.gem", dx, false);

    Model trex;
    trex.init("C:/Users/u5632744/Desktop/Computer Graphics/CGCoursework/resources/TRex.gem", dx, ModelAnim::ANIMATED);

    Plane plane;
    plane.init(dx);

    //Cube cube;
    //cube.init(dx);

    //Sphere sphere;
    //sphere.init(20, 20, 2.0f, dx);

    shaderAnim.init("VShaderAnim.hlsl", "PixelShader.hlsl", dx);
    shaderStat.init("VertexShader.hlsl", "PixelShader.hlsl", dx);

    AnimationInstance trexAnimInstance;
    trexAnimInstance.animation = &trex.animation;
    trexAnimInstance.resetAnimationTime();

    Matrix worldMatrix = Matrix();

    while (true) {
        float dt = timer.update();

        camera.update(win);
        Matrix view = camera.getViewMatrix();
        Matrix projection = projection.Projection(M_PI / 4.0f, float(win.width) / win.height, 0.1f, 100.0f);
        Matrix VP = projection.mul(view);

        dx.clear();

         //Draw Plane
        worldMatrix = worldMatrix.translation(vec3(0, -1, 0)); // Place the plane below the cubes
        shaderStat.updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        shaderStat.apply(dx);
        plane.geometry.draw(dx);

        //trexAnimInstance.update("Run", dt);
        //worldMatrix = worldMatrix.scaling(vec3(0.5f, 0.5f, 0.5f));
        //shader.updateConstantVS("animatedMeshBuffer", "W", &worldMatrix);
        //shader.updateConstantVS("animatedMeshBuffer", "bones", trexAnimInstance.matrices);
        //shader.apply(dx);
        //trex.draw(dx);

        //worldMatrix = worldMatrix.scaling(vec3(0.01f, 0.01f, 0.01f)) * worldMatrix.translation(vec3(0, 1, 400));
        //shader.updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        //shader.apply(dx);
        //soldier.draw(dx);

        worldMatrix = worldMatrix.scaling(vec3(0.01f, 0.01f, 0.01f)) * worldMatrix.translation(vec3(0, 1, 0));
        shaderStat.updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        shaderStat.apply(dx);
        acaciaTree.draw(dx);

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

        shaderStat.updateConstantVS("staticMeshBuffer", "VP", &VP);

        win.processMessages();
        dx.present();
    }
}
