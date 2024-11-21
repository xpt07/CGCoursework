#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <D3DCompiler.h>
#include <d3d11shader.h>
#include "DXCore.h"

#pragma comment(lib, "dxguid.lib")

class Shaders
{
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;


	std::string readFile(std::string filename);

	void compileVS(std::string VS_file, DXCore& core);
	void compilePS(std::string PS_file, DXCore& core);

	void apply(DXCore& core);

	void init(std::string VS_file, std::string PS_file, DXCore& core);
};

