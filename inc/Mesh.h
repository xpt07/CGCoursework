#pragma once
#include "core.h"
#include "DXCore.h"

struct Vertex {
	vec3 position;
	Colour colour;
};

class Triangle {
public:
	ID3D11Buffer* vertexBuffer;
    ID3D11RasterizerState* rasterizerState;

	void init(DXCore& core);
	void draw(DXCore& core);

private:
	int N = 3;
};