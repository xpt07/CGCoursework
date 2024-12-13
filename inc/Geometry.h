#pragma once
#include "GEMLoader.h"
#include "Animation.h"
#include "ShaderManager.h"

// Enum to differentiate between static and animated models
enum ModelType {
	STATIC,		// Models that do not animate
	ANIMATED	// Models with skeletal animation
};

// Structure representing a simple vertex with position and colour
struct Vertex {
	vec3 position;	// Position of the vertex in 3D space
	Colour colour;	// Colour associated with the vertex
};

// Class representing a simple triangle
class Triangle {
public:
	ID3D11Buffer* vertexBuffer;					// Stores vertex data for the triangle
	ID3D11RasterizerState* rasterizerState;		// Controls how the triangle is rendered

	// Initializes the vertex buffer with hardcoded vertex positions and colours.
	void init(DXCore& core);

	// Draws the triangle to the screen using the vertex buffer.
	void draw(DXCore& core);

private:
	int N = 3; // Defines the structure of a vertex for static meshes.
};

// Defines the structure of a vertex for static meshes.
struct STATIC_VERTEX
{
	vec3 pos;		// Position of the vertex
	vec3 normal;	// Normal for lighting calculation
	vec3 tangent;	// Tangent for normal mapping
	float tu, tv;	// Texture coordinates
};

// Defines the structure of a vertex for animated meshes.
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
	ID3D11Buffer* indexBuffer;		// Buffer holding the mesh's indices
	ID3D11Buffer* vertexBuffer;		// Buffer holding the mesh's vertices
	int indicesSize;				// Number of indices
	UINT strides;					// Size of each vertex in bytes

	// Initializes the mesh with raw vertex and index data.
	void init(void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices, DXCore& core);
	
	// Overload for static vertex initialization using vectors.
	void init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& core);

	// Overload for animated vertex initialization using vectors.
	void init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& core);

	// Draws the mesh using the vertex and index buffers.
	void draw(DXCore& core);
};

// Plane class generates a flat surface for rendering.
class Plane
{
public:
	Mesh geometry;	// Mesh representation of the plane

	// Helper function to create a STATIC_VERTEX for the plane.
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

	// Initializes the plane as a flat 2x2 quad with vertices and indices.
	void init(DXCore& core);
};

// Cube class generates a 3D cube for rendering.
class Cube {
public:
	Mesh geometry;	// Mesh representation of the cube

	// Helper function to create a STATIC_VERTEX for the cube
	STATIC_VERTEX addVertex(vec3 p, vec3 n, float tu, float tv) {
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = vec3();
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	// Initializes the cube by creating its vertices and indices.
	void init(DXCore& core);
};

// Sphere class generates a 3D sphere for rendering.
// The sphere is created by subdividing into rings and segments.
class Sphere {
public:
	Mesh geometry;	// Mesh representation of the sphere

	// Helper function to create a STATIC_VERTEX for the sphere.
	STATIC_VERTEX addVertex(vec3 p, vec3 n, float tu, float tv) {
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = vec3();
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	// Initializes the sphere with specified rings, segments, and radius.
	void init(int rings, int segments, float radius, DXCore& core);
};

// Model class represents a 3D model, which can consist of multiple meshes.
// Supports both static and animated models.
class Model {
public:
	std::vector<Mesh> meshes;						// List of meshes in the model
	std::vector<std::string> textureFilenames;		// Textures associated with the model
	Animation animation;							// Animation data for animated models
	ModelType type;									// Type of the model (STATIC or ANIMATED)

	// Initializes the model by loading data from a file.
	void init(std::string filename, DXCore& core, ModelType modelType);

	// Draws the model using the provided shaders and texture manager.
	void draw(DXCore& core, Shaders& shader, TextureManager& textureManager);
};