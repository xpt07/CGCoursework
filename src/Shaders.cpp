#include "../inc/Shaders.h"

std::string Shaders::readFile(std::string filename)
{
	std::ifstream myfile(filename);
	std::stringstream buffer;
	buffer << myfile.rdbuf();
	return buffer.str();
}

void Shaders::compileVS(std::string VS_file, DXCore& core)
{

	ID3DBlob* compiledVertexShader;
	ID3DBlob* status;
	HRESULT hr = D3DCompile(VS_file.c_str(), strlen(VS_file.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);
	core.device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	core.device->CreateInputLayout(layoutDesc, 2, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Vertex Shader Error", 0);
		exit(0);
	}
}

void Shaders::compilePS(std::string PS_file, DXCore& core)
{

	ID3DBlob* compiledPixelShader;
	ID3DBlob* status;
	HRESULT hr = D3DCompile(PS_file.c_str(), strlen(PS_file.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &compiledPixelShader, &status);
	core.device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), NULL, &pixelShader);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, (char*)status->GetBufferPointer(), "Pixel Shader Error", 0);
		exit(0);
	}
}

void Shaders::apply(DXCore& core)
{
	core.devicecontext->IASetInputLayout(layout);
	core.devicecontext->VSSetShader(vertexShader, NULL, 0);
	core.devicecontext->PSSetShader(pixelShader, NULL, 0);

}

void Shaders::init(std::string VS_file, std::string PS_file, DXCore& core)
{
	std::string vertexShaderHLSL = readFile(VS_file);
	std::string pixelShaderHLSL = readFile(PS_file);

	compileVS(vertexShaderHLSL, core);
	compilePS(pixelShaderHLSL, core);

}
