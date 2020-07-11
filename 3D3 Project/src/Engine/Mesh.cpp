#include "Mesh.h"

Mesh::Mesh(ID3D12Device5* device, std::vector<Vertex> vertices, UINT descriptorHeapIndex)
{
	this->vertices = vertices;
	this->descriptorHeapIndex = descriptorHeapIndex;

	this->resource = new Resource(device, this->GetSize(), RESOURCE_TYPE::UPLOAD, L"MESH_NONAME");

	this->resource->SetData(this->vertices.data());
}

Mesh::~Mesh()
{
	delete this->resource;
}

Resource* Mesh::GetResource() const
{
	return this->resource;
}

const size_t Mesh::GetSize() const
{
	return this->vertices.size() * sizeof(Vertex);
}

const size_t Mesh::GetNumVertices() const
{
	return this->vertices.size();
}

const UINT Mesh::GetDescriptorHeapIndex() const
{
	return this->descriptorHeapIndex;
}

