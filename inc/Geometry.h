// Geometry.h: Defines various classes and structures to handle 3D geometry, such as meshes, planes, cubes, and spheres,
// and their interaction with DirectX Core (DXCore) for rendering.

#pragma once
#include "GEMLoader.h"
#include "Animation.h"
#include "ShaderManager.h"

// Enum to differentiate between static and animated models
enum ModelType {
	STATIC, // Models that do not animate
	ANIMATED // Models with skeletal animation
};

// Structure representing a simple vertex with position and color
struct Vertex {
	vec3 position; // Position of the vertex in 3D space
	Colour colour; // Colour associated with the vertex
};

// Class representing a simple triangle
class Triangle {
public:
	ID3D11Buffer* vertexBuffer;				// Stores vertex data for the triangle
	ID3D11RasterizerState* rasterizerState; // Controls how the triangle is rendered

	// Initializes the vertex buffer with hardcoded vertex positions and colours.
	void init(DXCore& core);

	// Draws the triangle to the screen using the vertex buffer.
	void draw(DXCore& core);

private:
	int N = 3; // Number of vertices in the triangle
};

// Defines the structure of a vertex for static meshes.
struct STATIC_VERTEX
{
	vec3 pos;		// Position of the vertex
	vec3 normal;	// Normal for lighting calculations
	vec3 tangent;	// Tangent for normal mapping
	float tu, tv;	// Texture coordinates
};

// ADefines the structure of a vertex for animated meshes.
// Includes additional bone IDs and weights for skeletal animations.
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

// Represents a single 3D object with vertices and indices.
// Handles initialization and rendering for both static and animated models.
class Mesh
{
public:
	ID3D11Buffer* indexBuffer;	// Buffer holding the mesh's indices
	ID3D11Buffer* vertexBuffer;	// Buffer holding the mesh's vertices
	int indicesSize;			// Number of indices
	UINT strides;				// Size of each vertex in bytes

	// Initializes the mesh with raw vertex and index data.
	void init(void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices, DXCore& core);
	
	// Overload for static vertex initialization using vectors.
	void init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& core);

	// Overload for animated vertex initialization using vectors.
	void init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& core);

	// Draws the mesh using the vertex and index buffers.
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
	std::vector<std::string> textureFilenames;
	Animation animation;
	ModelType type;

	void init(std::string filename, DXCore& core, ModelType modelType);

	void draw(DXCore& core, Shaders& shader, TextureManager& textureManager);
};