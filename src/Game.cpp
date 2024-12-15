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

// Helper function to parse a vec3 from a comma-separated string
// This splits a string like "x,y,z" into a vec3.
vec3 parseVec3(const std::string& str) {
    std::istringstream stream(str);
    std::string token;
    float values[3] = { 0 };
    int i = 0;
    while (std::getline(stream, token, ',') && i < 3) {
        values[i++] = std::stof(token);
    }
    return vec3(values[0], values[1], values[2]);
}

// Loads the level data from a .txt file.
// It initializes the paths, settings, and attributes of all game objects like T-Rex, trees, camera, etc.
bool loadLevelData(
    const std::string& filename,
    std::string& trexMeshPath, ModelType& trexModelType, vec3& trexInitialPosition,
    vec3& planeScale, std::string& skyboxTexturePath, float& skyboxRadius,
    vec3& skylightDirection, float& skylightIntensity, vec3& skylightColor, vec3& ambientColor,
    vec3& cameraPosition, vec3& cameraForward, float& cameraSpeed, float& cameraSensitivity,
    std::string& pineMeshPath, ModelType& pineModelType, int& treeCount, float& treeMinScale, float& treeMaxScale, float& treeRadius
) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        MessageBoxA(nullptr, ("Failed to open level data file: " + filename).c_str(), "Error", MB_ICONERROR);
        return false;
    }

    std::string line, section;
    while (std::getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        // Determine which section of the file we're in
        if (line == "T-Rex" || line == "Plane" || line == "Skybox" || line == "Lighting" || line == "Camera" || line == "Trees") {
            section = line;
        }
        else {
            std::istringstream lineStream(line);
            std::string key, value;
            if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
                // Assign values based on the current section
                if (section == "T-Rex") {
                    if (key == "Mesh") trexMeshPath = value;
                    else if (key == "AnimationType") {
                        trexModelType = (value == "ANIMATED") ? ModelType::ANIMATED : ModelType::STATIC;
                    }
                    else if (key == "InitialPosition") {
                        trexInitialPosition = parseVec3(value);
                    }
                }
                else if (section == "Plane") {
                    if (key == "Scale") planeScale = parseVec3(value);
                }
                else if (section == "Skybox") {
                    if (key == "Texture") skyboxTexturePath = value;
                    else if (key == "Radius") skyboxRadius = std::stof(value);
                }
                else if (section == "Lighting") {
                    if (key == "SkyLightDirection") skylightDirection = parseVec3(value);
                    else if (key == "SkyLightIntensity") skylightIntensity = std::stof(value);
                    else if (key == "SkyLightColor") skylightColor = parseVec3(value);
                    else if (key == "AmbientColor") ambientColor = parseVec3(value);
                }
                else if (section == "Camera") {
                    if (key == "Position") cameraPosition = parseVec3(value);
                    else if (key == "Forward") cameraForward = parseVec3(value).normalize();
                    else if (key == "Speed") cameraSpeed = std::stof(value);
                    else if (key == "Sensitivity") cameraSensitivity = std::stof(value);
                }
                else if (section == "Trees") {
                    if (key == "Mesh") pineMeshPath = value;
                    else if (key == "ModelType") {
                        pineModelType = (value == "STATIC") ? ModelType::STATIC : ModelType::ANIMATED;
                    }
                    else if (key == "Count") treeCount = std::stoi(value);
                    else if (key == "MinScale") treeMinScale = std::stof(value);
                    else if (key == "MaxScale") treeMaxScale = std::stof(value);
                    else if (key == "Radius") treeRadius = std::stof(value);
                }
            }
        }
    }

    return true; // Successful loaded level data
}

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
    auto textureManager = std::make_unique<TextureManager>();

    // Random seed for tree placement
    srand(static_cast<unsigned>(time(0)));

    // Window and DXCore initialization
    win->init(1024, 1024, "CGCoursework");
    dx->init(win->width, win->height, win->hwnd, false);

    bool cameraControlEnabled = true; // Camera control starts enabled
    ShowCursor(FALSE); // Start with cursor hidden

    // Level data variables
    std::string trexMeshPath, skyboxTexturePath, pineMeshPath;
    ModelType trexModelType, pineModelType;
    vec3 trexInitialPosition, planeScale, skylightDirection, skylightColor, ambientColor;
    vec3 cameraPosition, cameraForward;
    int treeCount = 0;
    float skyboxRadius, skylightIntensity, cameraSpeed, cameraSensitivity;
    float treeMinScale, treeMaxScale, treeRadius;

    // Load level data
    if (!loadLevelData("level.txt", trexMeshPath, trexModelType, trexInitialPosition,
        planeScale, skyboxTexturePath, skyboxRadius,
        skylightDirection, skylightIntensity, skylightColor, ambientColor,
        cameraPosition, cameraForward, cameraSpeed, cameraSensitivity,
        pineMeshPath, pineModelType, treeCount, treeMinScale, treeMaxScale, treeRadius)) {
        return -1; // Exit if loading fails
    }

    // Initialize camera with loaded parameters
    auto camera = std::make_unique<Camera>(cameraPosition, cameraForward, cameraSpeed, cameraSensitivity);

    // Load T-Rex model
    auto trex = std::make_unique<Model>();
    trex->init(trexMeshPath, *dx, trexModelType);

    auto pine = std::make_unique<Model>();
    pine->init(pineMeshPath, *dx, pineModelType);

    auto plane = std::make_unique<Plane>();
    plane->init(*dx);

    // Initialize Skydome
    auto skydome = std::make_unique<Sphere>();
    skydome->init(30, 30, skyboxRadius, *dx); // Large sphere for Skydome

    // Initialize textures and shaders
    initializeTextures(*textureManager, *dx);
    initializeShaders(*shaderManager, *dx);

    // HDRI texture for Skydome
    ID3D11ShaderResourceView* skydomeTexture = textureManager->find(skyboxTexturePath);

    // Initialize T-Rex animation
    AnimationInstance trexAnimInstance;
    trexAnimInstance.animation = &trex->animation;
    vec3 trexPosition = trexInitialPosition; // Initial position of T-Rex

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

    // Generate trees based on loaded parameters
    std::vector<TreeInstance> trees = generateRandomTreesInRadius(treeCount, treeMinScale, treeMaxScale, treeRadius);

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
        shaderManager->getShader("shaderSkydome")->updateTexturePS("skyTex", skydomeTexture, *dx);
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
        float rotationAngle = acosf(cameraForward.dot(directionToCamera));
        if (cameraForward.cross(directionToCamera).y < 0) {
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
