#pragma once

#include <D3D11.h>
#include <D3Dcompiler.h>
#include <d3d11shader.h>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>

#include "DXCore.h" 

#pragma comment(lib, "dxguid.lib")

enum ShaderStage
{
	VertexShader,
	PixelShader
};

struct ConstantBufferVariable
{
	unsigned int offset;
	unsigned int size;
};

class ConstantBuffer
{
public:
	std::string name;
	std::map<std::string, ConstantBufferVariable> constantBufferData;
	ID3D11Buffer* cb;
	unsigned char* buffer;
	unsigned int cbSizeInBytes;
	int dirty;
	int index;
	ShaderStage shaderStage;
	void init(DXCore& core, unsigned int sizeInBytes, int constantBufferIndex, ShaderStage stage)
	{
		unsigned int sizeInBytes16 = ((sizeInBytes + 15) & -16);
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		bd.ByteWidth = sizeInBytes16;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		core.device->CreateBuffer(&bd, NULL, &cb);
		buffer = new unsigned char[sizeInBytes16];
		cbSizeInBytes = sizeInBytes;
		index = constantBufferIndex;
		dirty = 1;
		shaderStage = stage;
	}
	void update(std::string name, void* data)
	{
		ConstantBufferVariable cbVariable = constantBufferData[name];
		memcpy(&buffer[cbVariable.offset], data, cbVariable.size);
		dirty = 1;
	}
	void upload(DXCore& core)
	{
		if (dirty == 1)
		{
			D3D11_MAPPED_SUBRESOURCE mapped;
			core.devicecontext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
			memcpy(mapped.pData, buffer, cbSizeInBytes);
			core.devicecontext->Unmap(cb, 0);
			if (shaderStage == ShaderStage::VertexShader)
			{
				core.devicecontext->VSSetConstantBuffers(index, 1, &cb);
			}
			if (shaderStage == ShaderStage::PixelShader)
			{
				core.devicecontext->PSSetConstantBuffers(index, 1, &cb);
			}
			dirty = 0;
		}
	}
	void free()
	{
		cb->Release();
	}
};

class ConstantBufferReflection
{
public:
	void build(DXCore& core, ID3DBlob* shader, std::vector<ConstantBuffer>& buffers, std::map<std::string, int>& textureBindPoints, ShaderStage stage)
	{
		ID3D11ShaderReflection* reflection;
		D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
		D3D11_SHADER_DESC desc;
		reflection->GetDesc(&desc);
		for (int i = 0; i < desc.ConstantBuffers; i++)
		{
			ConstantBuffer buffer;
			ID3D11ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
			D3D11_SHADER_BUFFER_DESC cbDesc;
			constantBuffer->GetDesc(&cbDesc);
			buffer.name = cbDesc.Name;
			unsigned int totalSize = 0;
			for (int n = 0; n < cbDesc.Variables; n++)
			{
				ID3D11ShaderReflectionVariable* var = constantBuffer->GetVariableByIndex(n);
				D3D11_SHADER_VARIABLE_DESC vDesc;
				var->GetDesc(&vDesc);
				ConstantBufferVariable bufferVariable;
				bufferVariable.offset = vDesc.StartOffset;
				bufferVariable.size = vDesc.Size;
				buffer.constantBufferData.insert({ vDesc.Name, bufferVariable });
				totalSize += bufferVariable.size;
			}
			buffer.init(core, totalSize, i, stage);
			buffers.push_back(buffer);
		}
		for (int i = 0; i < desc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			reflection->GetResourceBindingDesc(i, &bindDesc);
			if (bindDesc.Type == D3D_SIT_TEXTURE)
			{
				textureBindPoints.insert({ bindDesc.Name, bindDesc.BindPoint });
			}
		}
	}
};