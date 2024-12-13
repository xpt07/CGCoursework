#include "../inc/Shaders.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string Shaders::readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf(); // Read the entire file into a string buffer.
    return buffer.str();    // Return the shader source as a string.
}

void Shaders::compileVS(const std::string& VS_filename, DXCore& core) {
    ID3DBlob* compiledVertexShader = nullptr;
    ID3DBlob* status = nullptr;

    // Compile the vertex shader source code.
    HRESULT hr = D3DCompile(
        VS_filename.c_str(),
        VS_filename.length(),
        nullptr,
        nullptr,
        nullptr,
        "VS",
        "vs_5_0",
        0,
        0,
        &compiledVertexShader,
        &status
    );

    if (FAILED(hr)) {
        if (status) {
            std::string errorMsg = static_cast<char*>(status->GetBufferPointer());
            status->Release();
            throw std::runtime_error("Vertex Shader Compilation Error: " + errorMsg);
        }
        throw std::runtime_error("Unknown error during Vertex Shader compilation.");
    }
    // Create the vertex shader
    core.device->CreateVertexShader(
        compiledVertexShader->GetBufferPointer(),
        compiledVertexShader->GetBufferSize(),
        nullptr,
        &vertexShader
    );

    // Define the vertex input layout to match the shader's input structure.
    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    // Create the input layout for the vertex shader.
    core.device->CreateInputLayout(
        layoutDesc,
        _countof(layoutDesc),
        compiledVertexShader->GetBufferPointer(),
        compiledVertexShader->GetBufferSize(),
        &layout
    );

    ConstantBufferReflection reflection;
    reflection.build(core, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);

    compiledVertexShader->Release();
}

void Shaders::compilePS(const std::string& PS_filename, DXCore& core) {
    ID3DBlob* compiledPixelShader = nullptr;
    ID3DBlob* status = nullptr;

    // Compile the pixel shader source code.
    HRESULT hr = D3DCompile(
        PS_filename.c_str(),
        PS_filename.length(),
        nullptr,
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        0,
        0,
        &compiledPixelShader,
        &status
    );

    if (FAILED(hr)) {
        if (status) {
            std::string errorMsg = static_cast<char*>(status->GetBufferPointer());
            status->Release();
            throw std::runtime_error("Pixel Shader Compilation Error: " + errorMsg);
        }
        throw std::runtime_error("Unknown error during Pixel Shader compilation.");
    }

    // Create the pixel shader
    core.device->CreatePixelShader(
        compiledPixelShader->GetBufferPointer(),
        compiledPixelShader->GetBufferSize(),
        nullptr,
        &pixelShader
    );

    ConstantBufferReflection reflection;
    reflection.build(core, compiledPixelShader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);

    compiledPixelShader->Release();
}

void Shaders::init(const std::string& VS_filename, const std::string& PS_filename, DXCore& core) {
    std::string vertexShaderSource = readFile(VS_filename);
    std::string pixelShaderSource = readFile(PS_filename);

    compileVS(vertexShaderSource, core);
    compilePS(pixelShaderSource, core);
}

void Shaders::updateConstantVS(const std::string& constantBufferName, const std::string& variableName, void* data) {
    updateConstant(constantBufferName, variableName, data, vsConstantBuffers);
}

void Shaders::updateConstantPS(const std::string& constantBufferName, const std::string& variableName, void* data) {
    updateConstant(constantBufferName, variableName, data, psConstantBuffers);
}

void Shaders::updateTexturePS(const std::string& textureName, ID3D11ShaderResourceView* srv, DXCore& core)
{
    // Retrieve the bind slot from the textureBindPointsPS map
    if (textureBindPointsPS.find(textureName) != textureBindPointsPS.end()) {
        int bindPoint = textureBindPointsPS[textureName];
        core.devicecontext->PSSetShaderResources(bindPoint, 1, &srv);
    }
    else {
        throw std::runtime_error("Texture bind point not found for: " + textureName);
    }
}

void Shaders::updateLight(const std::string& bufferName, vec3 lightDir, float intensity, vec3 skylightColor, vec3 ambientColor)
{
    struct LightData {
        vec3 LightDirection;
        float LightIntensity;
        vec3 SkylightColor;
        vec3 AmbientColor;
    };

    LightData light = { lightDir, intensity, skylightColor, ambientColor };

    for (auto& buffer : psConstantBuffers) {
        if (buffer.name == bufferName) {
            buffer.update("LightDirection", &light.LightDirection);
            buffer.update("LightIntensity", &light.LightIntensity);
            buffer.update("SkylightColor", &light.SkylightColor);
            buffer.update("AmbientColor", &light.AmbientColor);
            return;
        }
    }
}

void Shaders::updateConstant(const std::string& constantBufferName, const std::string& variableName, void* data, std::vector<ConstantBuffer>& buffers) {
    for (auto& buffer : buffers) {
        if (buffer.name == constantBufferName) {
            buffer.update(variableName, data);
            return;
        }
    }
}

void Shaders::apply(DXCore& core) {
    core.devicecontext->IASetInputLayout(layout);
    core.devicecontext->VSSetShader(vertexShader, nullptr, 0);
    core.devicecontext->PSSetShader(pixelShader, nullptr, 0);

    for (auto& buffer : vsConstantBuffers) {
        buffer.upload(core);
    }
    for (auto& buffer : psConstantBuffers) {
        buffer.upload(core);
    }
}
