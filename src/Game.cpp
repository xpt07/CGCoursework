#include "../inc/Window.h"
#include "../inc/Geometry.h"
#include "../inc/Timer.h"
#include "../inc/Camera.h"
#include "../inc/core.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <memory>

// Helper function to generate a random float within a range
static float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

// Structure to store information about a tree instance
struct TreeInstance {
    vec3 position;
    float scale;
};

// Generate random trees within a large circular radius
std::vector<TreeInstance> generateRandomTreesInRadius(
    int count, float minScale, float maxScale, float radius
) {
    std::vector<TreeInstance> trees;

    for (int i = 0; i < count; ++i) {
        float angle = randomFloat(0.0f, 2.0f * M_PI); // Random angle in radians
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

// Function to initialize textures
void initializeTextures(TextureManager& textureManager, DXCore& dx) {
    textureManager.load(dx, "Textures/T-rex_Base_Color.png");
    textureManager.load(dx, "Textures/bark09.png");
    textureManager.load(dx, "Textures/pine branch.png");
    textureManager.load(dx, "Textures/stump01.png");
}

// Function to initialize shaders
void initializeShaders(ShaderManager& shaderManager, DXCore& dx) {
    shaderManager.loadShader("shaderAnimTex", "VShaderAnim.hlsl", "TexPixelShader.hlsl", dx);
    shaderManager.loadShader("shaderStatTex", "VertexShader.hlsl", "TexPixelShader.hlsl", dx);
    shaderManager.loadShader("shaderStat", "VertexShader.hlsl", "PixelShader.hlsl", dx);
}

// Render trees
void renderTrees(const std::vector<TreeInstance>& trees, Model* pine, ShaderManager* shaderManager, DXCore& dx, TextureManager& textureManager) {
    for (const auto& tree : trees) {
        Matrix treeMatrix = Matrix::scaling(vec3(tree.scale, tree.scale, tree.scale)) *
            Matrix::translation(tree.position);
        shaderManager->getShader("shaderStatTex")->updateConstantVS("staticMeshBuffer", "W", &treeMatrix);
        shaderManager->applyShader("shaderStatTex", dx);
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
    auto camera = std::make_unique<Camera>(vec3(0, 2, -20));
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

    // Initialize textures and shaders
    initializeTextures(*textureManager, *dx);
    initializeShaders(*shaderManager, *dx);

    // Initialize T-Rex animation
    AnimationInstance trexAnimInstance;
    trexAnimInstance.animation = &trex->animation;
    trexAnimInstance.resetAnimationTime();

    // Generate random trees within a large radius
    const int treeCount = 50;          // Number of trees
    const float radius = 2000.0f;        // Radius of the circular area
    const float minTreeScale = 0.005f; // Minimum tree size
    const float maxTreeScale = 0.02f;  // Maximum tree size
    std::vector<TreeInstance> trees = generateRandomTreesInRadius(treeCount, minTreeScale, maxTreeScale, radius);

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

        Matrix view = camera->getViewMatrix();
        Matrix projection = projection.Projection(M_PI / 4.0f, float(win->width) / win->height, 0.1f, 100.0f);
        Matrix VP = projection.mul(view);

        dx->clear();

        // Draw Plane
        Matrix planeMatrix = Matrix::scaling(vec3(2.f, 2.f, 2.f)) * Matrix::translation(vec3(0, 0, 0));
        shaderManager->getShader("shaderStat")->updateConstantVS("staticMeshBuffer", "W", &planeMatrix);
        shaderManager->applyShader("shaderStat", *dx);
        plane->geometry.draw(*dx);

        // Render trees
        renderTrees(trees, pine.get(), shaderManager.get(), *dx, *textureManager);

        // Draw Animated T-Rex
        trexAnimInstance.update("roar", dt);
        Matrix trexMatrix = Matrix::translation(vec3(0, 0, 0));
        shaderManager->getShader("shaderAnimTex")->updateConstantVS("animatedMeshBuffer", "W", &trexMatrix);
        shaderManager->getShader("shaderAnimTex")->updateConstantVS("animatedMeshBuffer", "bones", trexAnimInstance.matrices);
        shaderManager->applyShader("shaderAnimTex", *dx);
        trex->draw(*dx, *shaderManager->getShader("shaderAnimTex"), *textureManager);

        // Update view-projection matrices
        shaderManager->getShader("shaderStatTex")->updateConstantVS("staticMeshBuffer", "VP", &VP);
        shaderManager->getShader("shaderStat")->updateConstantVS("staticMeshBuffer", "VP", &VP);
        shaderManager->getShader("shaderAnimTex")->updateConstantVS("animatedMeshBuffer", "VP", &VP);

        win->processMessages();
        dx->present();
    }
}
