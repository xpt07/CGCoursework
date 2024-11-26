#include "../inc/ShaderManager.h"
#include <fstream>

// Save the compiled shader blob to a file
void ShaderManager::saveShaderBlob(const std::string& blobFile, ID3DBlob* shaderBlob)
{
    D3DWriteBlobToFile(shaderBlob, std::wstring(blobFile.begin(), blobFile.end()).c_str(), false);
}

// Load a compiled shader blob from a file
ID3DBlob* ShaderManager::loadShaderBlob(const std::string& blobFile)
{
    ID3DBlob* shaderBlob = nullptr;
    D3DReadFileToBlob(std::wstring(blobFile.begin(), blobFile.end()).c_str(), &shaderBlob);
    return shaderBlob;
}

Shaders& ShaderManager::loadShader(const std::string& name, const std::string& vsFile, const std::string& psFile, DXCore& core)
{
    if (shaderMap.find(name) == shaderMap.end())
    {
        Shaders shader;

        // Try to load precompiled vertex shader
        ID3DBlob* vertexBlob = loadShaderBlob(vsFile + ".cso");
        if (!vertexBlob)
        {
            // If not available, compile and save
            std::string vertexShaderHLSL = shader.readFile(vsFile);
            shader.compileVS(vertexShaderHLSL, core);
            saveShaderBlob(vsFile + ".cso", vertexBlob);
        }

        // Try to load precompiled pixel shader
        ID3DBlob* pixelBlob = loadShaderBlob(psFile + ".cso");
        if (!pixelBlob)
        {
            // If not available, compile and save
            std::string pixelShaderHLSL = shader.readFile(psFile);
            shader.compilePS(pixelShaderHLSL, core);
            saveShaderBlob(psFile + ".cso", pixelBlob);
        }

        shaderMap[name] = shader;
    }

    return shaderMap[name];
}

Shaders& ShaderManager::getShader(const std::string& name)
{
    if (shaderMap.find(name) == shaderMap.end())
    {
        MessageBoxA(NULL, ("Shader not found: " + name).c_str(), "Error", MB_OK);
        exit(1);
    }

    return shaderMap[name];
}
