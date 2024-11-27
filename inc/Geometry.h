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

struct STATIC_VERTEX
{
	vec3 pos;
	vec3 normal;
	vec3 tangent;
	float tu;
	float tv;
};

class Mesh
{
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;

	void init(void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices, DXCore& core);
	void init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& core);

	void draw(DXCore& core);
};

class Plane
{
public:
	Mesh geometry;

	STATIC_VERTEX addVertex(vec3 p, vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = vec3();
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DXCore& core) {
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(vec3(-15, 0, -15), vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(vec3(15, 0, -15), vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(vec3(-15, 0, 15), vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(vec3(15, 0, 15), vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices = { 2, 1, 0, 1, 2, 3 };
		geometry.init(vertices, indices, core);

	}
};

class Model {
public:

};
