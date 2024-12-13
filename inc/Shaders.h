#pragma once
#include "core.h"
#include "Texture.h"
#include "ShaderReflection.h"

// Manages shader programs, their compilation, and the binding of resources like textures and constant buffers.
class Shaders {
public:
    // Initializes the shader by loading and compiling the vertex and pixel shaders.
    void init(const std::string& VS_filename, const std::string& PS_filename, DXCore& core);
    
    // Applies the compiled shaders and input layout to the rendering pipeline.
    void apply(DXCore& core);

    // Updates a constant buffer variable for the vertex shader.
    void updateConstantVS(const std::string& constantBufferName, const std::string& variableName, void* data);
   
    // Updates a constant buffer variable for the pixel shader.
    void updateConstantPS(const std::string& constantBufferName, const std::string& variableName, void* data);
    
    // Binds a texture to the pixel shader.
    void updateTexturePS(const std::string& textureName, ID3D11ShaderResourceView* srv, DXCore& core);
    
    // Updates lighting data in a specific pixel shader constant buffer.
    void updateLight(const std::string& bufferName, vec3 lightDir, float intensity, vec3 skylightColor, vec3 ambientColor);

private:
    ID3D11VertexShader* vertexShader = nullptr; // Compiled vertex shader.
    ID3D11PixelShader* pixelShader = nullptr;   // Compiled pixel shader.
    ID3D11InputLayout* layout = nullptr;        // Input layout for the vertex shader.
    ID3D11Buffer* instanceBuffer = nullptr;     // Buffer for instanced rendering.

    std::vector<ConstantBuffer> psConstantBuffers;  // Pixel shader constant buffers.
    std::vector<ConstantBuffer> vsConstantBuffers;  // Vertex shader constant buffers.
    std::map<std::string, int> textureBindPointsVS; // Texture bind points for the vertex shader.
    std::map<std::string, int> textureBindPointsPS; // Texture bind points for the pixel shader.

    // Reads shader source code from a file.
    std::string readFile(const std::string& filename);
    
    // Compiles the vertex shader and sets up the input layout.
    void compileVS(const std::string& VS_filename, DXCore& core);
    
    // Compiles the pixel shader.
    void compilePS(const std::string& PS_filename, DXCore& core);
    
    // Updates a specific variable in a constant buffer.
    void updateConstant(const std::string& constantBufferName, const std::string& variableName, void* data, std::vector<ConstantBuffer>& buffers);
};
