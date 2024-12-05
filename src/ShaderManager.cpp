#include "../inc/ShaderManager.h"

void ShaderManager::loadShader(const std::string& name, const std::string& vsFile, const std::string& psFile, DXCore& core) {
    Shaders shader;
    shader.init(vsFile, psFile, core);
    shaders[name] = shader;
}

Shaders* ShaderManager::getShader(const std::string& name) {
    if (shaders.find(name) != shaders.end()) {
        return &shaders[name];
    }
    return nullptr;
}

void ShaderManager::applyShader(const std::string& name, DXCore& core) {
    Shaders* shader = getShader(name);
    if (shader) {
        shader->apply(core);
    }
}
