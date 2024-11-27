#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <D3DCompiler.h>
#include <d3d11shader.h>
#include "ShaderReflection.h"
#include "DXCore.h"

#pragma comment(lib, "dxguid.lib")

class Shaders
{
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;

	std::vector<ConstantBuffer> psConstantBuffers;
	std::vector<ConstantBuffer> vsConstantBuffers;
	std::map<std::string, int> textureBindPointsVS;
	std::map<std::string, int> textureBindPointsPS;


	std::string readFile(std::string filename);

	void compileVS(std::string VS_file, DXCore& core);
	void compilePS(std::string PS_file, DXCore& core);

	void updateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers)
	{
		for (int i = 0; i < buffers.size(); i++)
		{
			if (buffers[i].name == constantBufferName)
			{
				buffers[i].update(variableName, data);
				return;
			}
		}
	}

	void updateConstantVS(std::string constantBufferName, std::string variableName, void* data)
	{
		updateConstant(constantBufferName, variableName, data, vsConstantBuffers);
	}
	void updateConstantPS(std::string constantBufferName, std::string variableName, void* data)
	{
		updateConstant(constantBufferName, variableName, data, psConstantBuffers);
	}

	void apply(DXCore& core);

	void init(std::string VS_file, std::string PS_file, DXCore& core);
};

