#include "Mesh.h"

Mesh::Mesh(ID3D12Device5* device, std::vector<Vertex> vertices, UINT vertexDataIndex)
{
	this->vertices = vertices;
	this->vertexDataIndex = vertexDataIndex;

	this->resource = new Resource(device, this->GetSize(), RESOURCE_TYPE::UPLOAD, L"MESH_NONAME");

	this->resource->SetData(this->vertices.data());
}

Mesh::~Mesh()
{
	delete this->resource;
}

Resource* Mesh::GetResource()
{
	return this->resource;
}

size_t Mesh::GetSize()
{
	size_t hello = this->vertices.size() * sizeof(Vertex);
	return hello;
}

size_t Mesh::GetNumVertices()
{
	return this->vertices.size();
}

UINT Mesh::GetVertexDataIndex()
{
	return this->vertexDataIndex;
}

