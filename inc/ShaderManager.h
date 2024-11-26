#pragma once
#include <map>
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Shaders.h"

class ShaderManager
{
private:
    std::map<std::string, Shaders> shaderMap;

public:
    Shaders& loadShader(const std::string& name, const std::string& vsFile, const std::string& psFile, DXCore& core);
    Shaders& getShader(const std::string& name);

    void saveShaderBlob(const std::string& blobFile, ID3DBlob* shaderBlob);
    ID3DBlob* loadShaderBlob(const std::string& blobFile);
};
