#pragma once
#include "Shaders.h"
#include "DXCore.h"

class ShaderManager {
public:
    void loadShader(const std::string& name, const std::string& vsFile, const std::string& psFile, DXCore& core);
    Shaders* getShader(const std::string& name);
    void applyShader(const std::string& name, DXCore& core);

private:
    std::map<std::string, Shaders> shaders;
};
