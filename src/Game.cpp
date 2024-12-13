#include "../inc/Window.h"
#include "../inc/Geometry.h"
#include "../inc/Timer.h"
#include "../inc/Camera.h"
#include "../inc/core.h"
#include "../inc/AnimationController.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <memory>

// Helper function to calculate distance between two 3D points
static float calculateDistance(const vec3& a, const vec3& b) {
    return (a - b).getLength();
}

// Helper function to generate a random float within a range
static float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

// Structure to store information about a tree instance
struct TreeInstance {
    vec3 position;  // Position of the tree
    float scale;    // Scale of the tree
};

// Generate a set of randomly placed trees within a specified circular area
std::vector<TreeInstance> generateRandomTreesInRadius(
    int count, float minScale, float maxScale, float radius
) {
    std::vector<TreeInstance> trees;

    for (int i = 0; i < count; ++i) {
        float angle = randomFloat(0.0f, 2.0f * M_PI); // Random angle
        float distance = radius * sqrt(randomFloat(0.0f, 1.0f));  // Square root for even distribution

        TreeInstance tree;
        tree.position = vec3(
            distance * cos(angle),  // x = r * cos(theta)
            0.0f,                   // Fixed y for ground level
            distance * sin(angle)   // z = r * sin(theta)
        );

        tree.scale = randomFloat(minScale, maxScale); // Random scale for the tree
        trees.push_back(tree);
    }

    return trees;
}

// Load textures into the texture manager
void initializeTextures(TextureManager& textureManager, DXCore& dx) {
    textureManager.load(dx, "Textures/T-rex_Base_Color.png");
    textureManager.load(dx, "Textures/bark09.png");
    textureManager.load(dx, "Textures/pine branch.png");
    textureManager.load(dx, "Textures/stump01.png");
    textureManager.load(dx, "resources/NightSkyHDRI001_4K-TONEMAPPED.jpg"); // HDRI texture for the Skydome
}

// Load and compile the shaders.
void initializeShaders(ShaderManager& shaderManager, DXCore& dx) {
    shaderManager.loadShader("shaderAnimTex", "VShaderAnim.hlsl", "TexPixelShader.hlsl", dx);
    shaderManager.loadShader("shaderStatTex", "VertexShader.hlsl", "TexPixelShader.hlsl", dx);
    shaderManager.loadShader("shaderStat", "VertexShader.hlsl", "PixelShader.hlsl", dx);
    shaderManager.loadShader("shaderSkydome", "SkydomeVertexShader.hlsl", "SkydomePixelShader.hlsl", dx);
}

// Render trees
void renderTrees(const std::vector<TreeInstance>& trees, Model* pine, ShaderManager* shaderManager, DXCore& dx, TextureManager& textureManager) {
    for (const auto& tree : trees) {
        // Create transformation matrix for each tree
        Matrix treeMatrix = Matrix::scaling(vec3(tree.scale, tree.scale, tree.scale)) *
            Matrix::translation(tree.position);

        // Update shader with tree's transformation
        shaderManager->getShader("shaderStatTex")->updateConstantVS("staticMeshBuffer", "W", &treeMatrix);
        shaderManager->applyShader("shaderStatTex", dx);
        
        // Render the tree model
        pine->draw(dx, *shaderManager->getShader("shaderStatTex"), textureManager);
    }
}

// Main game function
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    // Initialize core systems
    auto dx = std::make_unique<DXCore>();
    auto win = std::make_unique<Window>();
    auto shaderManager = std::make_unique<ShaderManager>();
    auto timer = std::make_unique<Timer>();
    auto camera = std::make_unique<Camera>(vec3(0, 2, -50));
    auto textureManager = std::make_unique<TextureManager>();

    // Random seed for tree placement
    srand(static_cast<unsigned>(time(0)));

    // Window and DXCore initialization
    win->init(1024, 1024, "CGCoursework");
    dx->init(win->width, win->height, win->hwnd, false);

    bool cameraControlEnabled = true; // Camera control starts enabled
    ShowCursor(FALSE); // Start with cursor hidden

    // Load models
    auto trex = std::make_unique<Model>();
    trex->init("resources/TRex.gem", *dx, ModelType::ANIMATED);

    auto pine = std::make_unique<Model>();
    pine->init("resources/Pine/pine.gem", *dx, ModelType::STATIC);

    auto plane = std::make_unique<Plane>();
    plane->init(*dx);

    // Initialize Skydome
    auto skydome = std::make_unique<Sphere>();
    skydome->init(30, 30, 100.0f, *dx); // Large sphere for Skydome

    // Initialize textures and shaders
    initializeTextures(*textureManager, *dx);
    initializeShaders(*shaderManager, *dx);

    // HDRI texture for Skydome
    ID3D11ShaderResourceView* skydomeTexture = textureManager->find("resources/NightSkyHDRI001_4K-TONEMAPPED.jpg");

    // Initialize T-Rex animation
    AnimationInstance trexAnimInstance;
    trexAnimInstance.animation = &trex->animation;
    vec3 trexPosition = vec3(0, 0, 50); // Initial position of T-Rex

    // Animation controller setup
    AnimationController animationController;
    animationController.addState("Idle", [&]() {
        trexAnimInstance.resetAnimationTime();
        trexAnimInstance.currentAnimation = "Idle";
        });

    animationController.addState("Run", [&]() {
        trexAnimInstance.resetAnimationTime();
        trexAnimInstance.currentAnimation = "Run";
        });

    animationController.addState("attack", [&]() {
        trexAnimInstance.resetAnimationTime();
        trexAnimInstance.currentAnimation = "attack";
        });

    // Lighting setup
    vec3 skylightDirection = vec3(0.0f, 1.0f, 0.0f); // Downward
    float skylightIntensity = 0.1f;                  // Slightly increased intensity
    vec3 skylightColor = vec3(0.1f, 0.1f, 0.15f);     // Dark blue skylight color, slightly brighter
    vec3 ambientColor = vec3(0.1f, 0.1f, 0.15f);     // Dim bluish-gray ambient light for overall visibility

    // Generate random trees within a large radius
    const int treeCount = 100;          // Number of trees
    const float radius = 9000.0f;        // Radius of the circular area
    const float minTreeScale = 0.005f; // Minimum tree size
    const float maxTreeScale = 0.02f;  // Maximum tree size
    std::vector<TreeInstance> trees = generateRandomTreesInRadius(treeCount, minTreeScale, maxTreeScale, radius);

    Matrix worldMatrix;

    while (true) {
        float dt = timer->update();

        // Handle escape key for toggling camera control
        if (win->keys[VK_ESCAPE]) {
            cameraControlEnabled = !cameraControlEnabled; // Toggle control mode
            ShowCursor(!cameraControlEnabled); // Show/hide cursor
            win->keys[VK_ESCAPE] = false; // Reset key state
        }

        if (cameraControlEnabled) {
            camera->update(*win, dt); // Update camera only if control is enabled
        }

        // Compute View-Projection matrix
        Matrix view = camera->getViewMatrix();
        Matrix projection = projection.Projection(M_PI / 4.0f, float(win->width) / win->height, 0.1f, 100.0f);
        Matrix VP = projection.mul(view);

        dx->clear();

        // Update lighting
        shaderManager->getShader("shaderStatTex")->updateLight("LightBuffer", skylightDirection, skylightIntensity, skylightColor, ambientColor);
        shaderManager->getShader("shaderAnimTex")->updateLight("LightBuffer", skylightDirection, skylightIntensity, skylightColor, ambientColor);
        shaderManager->getShader("shaderStat")->updateLight("LightBuffer", skylightDirection, skylightIntensity, skylightColor, ambientColor);

        // Render Skydome
        worldMatrix =  Matrix::translation(vec3(camera->position));
        shaderManager->getShader("shaderSkydome")->updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        shaderManager->getShader("shaderSkydome")->updateConstantVS("staticMeshBuffer", "VP", &VP);
        shaderManager->getShader("shaderSkydome")->updateTexturePS("skyTex", skydomeTexture, *dx); // Bind HDRI texture
        shaderManager->applyShader("shaderSkydome", *dx);
        skydome->geometry.draw(*dx);

        // Draw Plane
        worldMatrix = Matrix::scaling(vec3(10.f, 10.f, 10.f));
        shaderManager->getShader("shaderStat")->updateConstantVS("staticMeshBuffer", "W", &worldMatrix);
        shaderManager->applyShader("shaderStat", *dx);
        plane->geometry.draw(*dx);

        // Render trees
        renderTrees(trees, pine.get(), shaderManager.get(), *dx, *textureManager);

        // Handle T-Rex animations based on player distance
        float distanceToCamera = calculateDistance(trexPosition, camera->position);
        if (distanceToCamera < 10.f) {
            animationController.transitionTo("attack");
        }
        else if (distanceToCamera < 80.0f) {
            animationController.transitionTo("Run");
            vec3 directionToCamera = (camera->position - trexPosition).normalize();
            trexPosition += directionToCamera * dt * 5.0f; // Move toward the camera
        }
        else {
            animationController.transitionTo("Idle");
        }

        // Update T-Rex animation
        trexAnimInstance.update(animationController.getCurrentState(), dt);

        // Calculate the direction vector to the camera, projected to the XZ-plane
        vec3 directionToCamera = camera->position - trexPosition;
        directionToCamera.y = 0.0f; // Ignore vertical component
        directionToCamera = directionToCamera.normalize();

        // Calculate T-Rex orientation towards the camera
        vec3 directionToCamera = (camera->position - trexPosition).normalize();
        vec3 forwardDirection(0, 0, 1); // Default forward direction
        float rotationAngle = acosf(forwardDirection.dot(directionToCamera));
        if (forwardDirection.cross(directionToCamera).y < 0) {
            rotationAngle = -rotationAngle; // Adjust rotation for clockwise/counterclockwise
        }

        // Apply transformations to T-Rex model
        worldMatrix = Matrix::translation(vec3(trexPosition.x, 0, trexPosition.z)) * Matrix::RotateY(rotationAngle);
        shaderManager->getShader("shaderAnimTex")->updateConstantVS("animatedMeshBuffer", "W", &worldMatrix);
        shaderManager->getShader("shaderAnimTex")->updateConstantVS("animatedMeshBuffer", "bones", trexAnimInstance.matrices);
        shaderManager->applyShader("shaderAnimTex", *dx);
        trex->draw(*dx, *shaderManager->getShader("shaderAnimTex"), *textureManager);

        // Update view-projection matrices
        shaderManager->getShader("shaderStatTex")->updateConstantVS("staticMeshBuffer", "VP", &VP);
        shaderManager->getShader("shaderStat")->updateConstantVS("staticMeshBuffer", "VP", &VP);
        shaderManager->getShader("shaderAnimTex")->updateConstantVS("animatedMeshBuffer", "VP", &VP);

        win->processMessages(); // Handle window events
        dx->present();          // Present the rendered frame
    }
}
