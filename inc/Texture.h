#pragma once
#include <string>
#include <map>
#include "DXCore.h"
class Texture {
public:
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;
	ID3D11RenderTargetView* rtv;

	void init(int width, int height, int channels, DXGI_FORMAT format, unsigned char *data, DXCore& core);
	void load(DXCore& core, std::string filename);

	void free() {
		srv->Release();
		texture->Release();
	}
};

class TextureManager
{
public:
	std::map<std::string, Texture*> textures;

	void load(DXCore& core, std::string filename)
	{
		std::map<std::string, Texture*>::iterator it = textures.find(filename);
		if (it != textures.end())
		{
			return;
		}
		Texture* texture = new Texture();
		texture->load(core, filename);
		textures.insert({ filename, texture });
	}
	ID3D11ShaderResourceView* find(std::string name)
	{
		return textures[name]->srv;
	}
	void unload(std::string name)
	{
		textures[name]->free();
		textures.erase(name);
	}
	~TextureManager()
	{
		for (auto it = textures.cbegin(); it != textures.cend(); )
		{
			it->second->free();
			textures.erase(it++);
		}
	}
};

class Sampler {
public:
	ID3D11SamplerState* state;

	void init(DXCore& core);
};