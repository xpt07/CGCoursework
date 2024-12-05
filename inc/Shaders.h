#pragma once
#include <string>
#include <vector>
#include <map>
#include "DXCore.h"
#include "ShaderReflection.h"

class Shaders {
public:
    void init(const std::string& VS_filename, const std::string& PS_filename, DXCore& core);
    void apply(DXCore& core);

    void updateConstantVS(const std::string& constantBufferName, const std::string& variableName, void* data);
    void updateConstantPS(const std::string& constantBufferName, const std::string& variableName, void* data);

private:
    ID3D11VertexShader* vertexShader = nullptr;
    ID3D11PixelShader* pixelShader = nullptr;
    ID3D11InputLayout* layout = nullptr;

    std::vector<ConstantBuffer> psConstantBuffers;
    std::vector<ConstantBuffer> vsConstantBuffers;
    std::map<std::string, int> textureBindPointsVS;
    std::map<std::string, int> textureBindPointsPS;

    std::string readFile(const std::string& filename);
    void compileVS(const std::string& VS_filename, DXCore& core);
    void compilePS(const std::string& PS_filename, DXCore& core);
    void updateConstant(const std::string& constantBufferName, const std::string& variableName, void* data, std::vector<ConstantBuffer>& buffers);
};
