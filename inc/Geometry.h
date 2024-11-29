#pragma once
#include "GEMLoader.h"
#include "Animation.h"
#include "core.h"
#include "Shaders.h"
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

struct ANIMATED_VERTEX
{
	vec3 pos;
	vec3 normal;
	vec3 tangent;
	float tu;
	float tv;
	unsigned int bonesIDs[4];
	float boneWeights[4];
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

	void init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& core)
	{
		init(&vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size(), core);
	}


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
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		geometry.init(vertices, indices, core);

	}
};

class Cube {
public:
	Mesh geometry;

	STATIC_VERTEX addVertex(vec3 p, vec3 n, float tu, float tv) {
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = vec3();
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DXCore& core);
};

class Sphere {
public:
	Mesh geometry;

	STATIC_VERTEX addVertex(vec3 p, vec3 n, float tu, float tv) {
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = vec3();
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(int rings, int segments, float radius, DXCore& core);
};

class Model {
public:
	std::vector<Mesh> meshes;
	Animation animation;

	void init(std::string filename, DXCore& core, bool isAnimated);

	void draw(DXCore& core);
};