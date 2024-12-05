#include "../inc/Shaders.h"

std::string Shaders::readFile(std::string filename)
{
	std::ifstream myfile(filename);
	std::stringstream buffer;
	buffer << myfile.rdbuf();
	return buffer.str();
}

void Shaders::compileVS(std::string VS_filename, DXCore& core)
{

	ID3DBlob* compiledVertexShader = nullptr;
	ID3DBlob* status = nullptr;

	HRESULT hr = D3DCompile(VS_filename.c_str(),
		strlen(VS_filename.c_str()),
		NULL,
		NULL,
		NULL,
		"VS",
		"vs_5_0",
		0,
		0,
		&compiledVertexShader,
		&status);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
		exit(0);
	}

	core.device->CreateVertexShader(
		compiledVertexShader->GetBufferPointer(),
		compiledVertexShader->GetBufferSize(),
		NULL,
		&vertexShader);

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	core.device->CreateInputLayout(layoutDesc, 
		6, 
		compiledVertexShader->GetBufferPointer(), 
		compiledVertexShader->GetBufferSize(), 
		&layout);

	ConstantBufferReflection reflection;
	reflection.build(core, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
}

void Shaders::compilePS(std::string PS_filename, DXCore& core)
{

	ID3DBlob* compiledPixelShader = nullptr;
	ID3DBlob* status = nullptr;

	HRESULT hr = D3DCompile(PS_filename.c_str(),
		strlen(PS_filename.c_str()),
		NULL,
		NULL,
		NULL,
		"PS",
		"ps_5_0",
		0,
		0,
		&compiledPixelShader,
		&status);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Pixel Shader Error", 0);
		exit(0);
	}

	core.device->CreatePixelShader(
		compiledPixelShader->GetBufferPointer(),
		compiledPixelShader->GetBufferSize(),
		NULL,
		&pixelShader);

	ConstantBufferReflection reflection;
	reflection.build(core, compiledPixelShader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);

}

void Shaders::updateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers)
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

void Shaders::updateConstantVS(std::string constantBufferName, std::string variableName, void* data)
{
	updateConstant(constantBufferName, variableName, data, vsConstantBuffers);
}
void Shaders::updateConstantPS(std::string constantBufferName, std::string variableName, void* data)
{
	updateConstant(constantBufferName, variableName, data, psConstantBuffers);
}

void Shaders::apply(DXCore& core)
{
	core.devicecontext->IASetInputLayout(layout);
	core.devicecontext->VSSetShader(vertexShader, NULL, 0);
	core.devicecontext->PSSetShader(pixelShader, NULL, 0);

	for (int i = 0; i < vsConstantBuffers.size(); i++)
	{
		vsConstantBuffers[i].upload(core);
	}
	for (int i = 0; i < psConstantBuffers.size(); i++)
	{
		psConstantBuffers[i].upload(core);
	}
}

void Shaders::init(std::string VS_filename, std::string PS_filename, DXCore& core)
{
	std::string vertexShaderHLSL = readFile(VS_filename);
	std::string pixelShaderHLSL = readFile(PS_filename);

	compileVS(vertexShaderHLSL, core);
	compilePS(pixelShaderHLSL, core);

}
