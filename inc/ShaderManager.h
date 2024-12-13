#pragma once
#include "Shaders.h"
#include "DXCore.h"

class ShaderManager {
public:
    // Loads and initializes a shader with the given name, vertex shader file, and pixel shader file.
    void loadShader(const std::string& name, const std::string& vsFile, const std::string& psFile, DXCore& core);
    
    // Retrieves a pointer to the shader associated with the given name.
    // Returns nullptr if the shader is not found.
    Shaders* getShader(const std::string& name);
    
    // Applies the shader associated with the given name to the rendering pipeline.
    void applyShader(const std::string& name, DXCore& core);

private:
    std::map<std::string, Shaders> shaders; // Stores shaders by name for quick access.
};
