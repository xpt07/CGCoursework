#include "../inc/ShaderManager.h"

void ShaderManager::loadShader(const std::string& name, const std::string& vsFile, const std::string& psFile, DXCore& core) {
    Shaders shader;                         // Create a new shader instance.
    shader.init(vsFile, psFile, core);      // Initialize the shader with the given files.
    shaders[name] = shader;                 // Store the shader in the map using the name as the key.
}

Shaders* ShaderManager::getShader(const std::string& name) {
    if (shaders.find(name) != shaders.end()) {      // Check if the shader exists in the map.
        return &shaders[name];                      // Return a pointer to the shader.
    }
    return nullptr;                                 // Return nullptr if the shader is not found.
}

void ShaderManager::applyShader(const std::string& name, DXCore& core) {
    Shaders* shader = getShader(name);  // Retrieve the shader by name.
    if (shader) {                       // Check if the shader was found.
        shader->apply(core);            // Apply the shader to the pipeline.
    }
}
