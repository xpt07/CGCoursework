#include "../inc/Geometry.h"

void Mesh::init(void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices, DXCore& core)
{
	D3D11_BUFFER_DESC bd;
	memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * numIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = indices;
	core.device->CreateBuffer(&bd, &data, &indexBuffer);
	bd.ByteWidth = vertexSizeInBytes * numVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.pSysMem = vertices;
	core.device->CreateBuffer(&bd, &data, &vertexBuffer);
	indicesSize = numIndices;
	strides = vertexSizeInBytes;

}

void Mesh::init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXCore& core)
{
	init(&vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size(), core);
}

void Mesh::draw(DXCore& core)
{
	UINT offsets = 0;
	core.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	core.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	core.devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	core.devicecontext->DrawIndexed(indicesSize, 0, 0);
}

void Triangle::init(DXCore& core) {
	Vertex vertices[3];
	vertices[0].position = vec3(0, 1.0f, 0);
	vertices[0].colour = Colour(0, 1.0f, 0);
	vertices[1].position = vec3(-1.0f, -1.0f, 0);
	vertices[1].colour = Colour(1.0f, 0, 0);
	vertices[2].position = vec3(1.0f, -1.0f, 0);
	vertices[2].colour = Colour(0, 0, 1.0f);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA uploadData;
	bd.ByteWidth = sizeof(Vertex) * N;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	uploadData.pSysMem = vertices;
	uploadData.SysMemPitch = 0;
	uploadData.SysMemSlicePitch = 0;
	core.device->CreateBuffer(&bd, &uploadData, &vertexBuffer);

}

void Triangle::draw(DXCore& core)
{
	UINT offsets;
	offsets = 0;
	UINT strides = sizeof(Vertex);
	core.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	core.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	core.devicecontext->Draw(3, 0);

}

void Cube::init(DXCore& core) {
	std::vector<STATIC_VERTEX> vertices;

	vec3 p0 = vec3(-1.0f, -1.0f, -1.0f);
	vec3 p1 = vec3(1.0f, -1.0f, -1.0f);
	vec3 p2 = vec3(1.0f, 1.0f, -1.0f);
	vec3 p3 = vec3(-1.0f, 1.0f, -1.0f);
	vec3 p4 = vec3(-1.0f, -1.0f, 1.0f);
	vec3 p5 = vec3(1.0f, -1.0f, 1.0f);
	vec3 p6 = vec3(1.0f, 1.0f, 1.0f);
	vec3 p7 = vec3(-1.0f, 1.0f, 1.0f);

	vertices.push_back(addVertex(p0, vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p1, vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p2, vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p3, vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));

	vertices.push_back(addVertex(p5, vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p4, vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p7, vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p6, vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));

	vertices.push_back(addVertex(p4, vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p0, vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p3, vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p7, vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

	vertices.push_back(addVertex(p1, vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p5, vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p6, vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p2, vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

	vertices.push_back(addVertex(p3, vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p2, vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p6, vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p7, vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));

	vertices.push_back(addVertex(p4, vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p5, vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p1, vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p0, vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));

	std::vector<unsigned int> indices;
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);
	indices.push_back(4); indices.push_back(5); indices.push_back(6);
	indices.push_back(4); indices.push_back(6); indices.push_back(7);
	indices.push_back(8); indices.push_back(9); indices.push_back(10);
	indices.push_back(8); indices.push_back(10); indices.push_back(11);
	indices.push_back(12); indices.push_back(13); indices.push_back(14);
	indices.push_back(12); indices.push_back(14); indices.push_back(15);
	indices.push_back(16); indices.push_back(17); indices.push_back(18);
	indices.push_back(16); indices.push_back(18); indices.push_back(19);
	indices.push_back(20); indices.push_back(21); indices.push_back(22);
	indices.push_back(20); indices.push_back(22); indices.push_back(23);

	geometry.init(vertices, indices, core);
}

void Sphere::init(int rings, int segments, float radius, DXCore& core) {
	std::vector<STATIC_VERTEX> vertices;
	std::vector<unsigned int> indices;

	for (int lat = 0; lat <= rings; ++lat) {
		float theta = lat * M_PI / rings;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);

		for (int lon = 0; lon <= segments; ++lon) {
			float phi = lon * 2.0f * M_PI / segments;
			float sinPhi = sinf(phi);
			float cosPhi = cosf(phi);

			vec3 position = vec3(
				radius * sinTheta * cosPhi,
				radius * cosTheta,
				radius * sinTheta * sinPhi
			);
			vec3 normal = position.normalize();
			float tu = 1.0f - (float)lon / segments;
			float tv = 1.0f - (float)lat / rings;

			vertices.push_back(addVertex(position, normal, tu, tv));
		}
	}

	for (int lat = 0; lat < rings; ++lat) {
		for (int lon = 0; lon < segments; ++lon) {
			int current = lat * (segments + 1) + lon;
			int next = current + segments + 1;

			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(current + 1);

			indices.push_back(current + 1);
			indices.push_back(next);
			indices.push_back(next + 1);
		}
	}

	geometry.init(vertices, indices, core);
}

void Model::init(std::string filename, DXCore& core, bool isAnimated)
{

	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	GEMLoader::GEMAnimation gemanimation;

	loader.load(filename, gemmeshes, gemanimation);

	for (int i = 0; i < gemmeshes.size(); i++) {
		Mesh mesh;
		std::vector<ANIMATED_VERTEX> vertices;

		for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
			ANIMATED_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
			vertices.push_back(v);
		}

		mesh.init(vertices, gemmeshes[i].indices, core);
		meshes.push_back(mesh);
	}

	for (int i = 0; i < gemanimation.bones.size(); i++)
	{
		Bone bone;
		bone.name = gemanimation.bones[i].name;
		memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
		bone.parentIndex = gemanimation.bones[i].parentIndex;
		animation.skeleton.bones.push_back(bone);
	}

	for (int i = 0; i < gemanimation.animations.size(); i++)
	{
		std::string name = gemanimation.animations[i].name;
		AnimationSequence aseq;
		aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
		for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
		{
			AnimationFrame frame;
			for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
			{
				vec3 p;
				Quaternion q;
				vec3 s;
				memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(vec3));
				frame.positions.push_back(p);
				memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
				frame.rotations.push_back(q);
				memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(vec3));
				frame.scales.push_back(s);
			}
			aseq.frames.push_back(frame);
		}
		animation.animations.insert({ name, aseq });
	}

	//GEMLoader::GEMModelLoader loader;
	//std::vector<GEMLoader::GEMMesh> gemmeshes;
	//loader.load(filename, gemmeshes);
	//for (int i = 0; i < gemmeshes.size(); i++) {
	//	Mesh mesh;
	//	std::vector<STATIC_VERTEX> vertices;
	//	if (isAnimated)
	//	{
	//		for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
	//			STATIC_VERTEX v;
	//			memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(STATIC_VERTEX));
	//			vertices.push_back(v);
	//		}
	//	}
	//	else
	//	{
	//		for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
	//			STATIC_VERTEX v;
	//			memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
	//			vertices.push_back(v);
	//		}
	//	}
	//	mesh.init(vertices, gemmeshes[i].indices, core);
	//	meshes.push_back(mesh);
	//}

}

void Model::draw(DXCore& core)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(core);
	}
}
