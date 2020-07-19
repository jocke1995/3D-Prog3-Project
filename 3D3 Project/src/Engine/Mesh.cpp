#include "Mesh.h"

Mesh::Mesh(ID3D12Device5* device, std::vector<Vertex> vertices, std::vector<unsigned int> indices, UINT descriptorHeapIndex)
{
	this->vertices = vertices;
	this->indices = indices;
	this->descriptorHeapIndex = descriptorHeapIndex;

	// Set vertices
	this->resourceVertices = new Resource(device, this->GetSizeOfVertices(), RESOURCE_TYPE::UPLOAD, L"Vertex Resource");
	this->resourceVertices->SetData(this->vertices.data());

	// Set indices
	this->resourceIndices = new Resource(device, this->GetSizeOfIndices(), RESOURCE_TYPE::UPLOAD, L"Index Resource");
	this->resourceIndices->SetData(this->indices.data());
	this->CreateIndexBufferView();
}

Mesh::~Mesh()
{
	delete this->resourceVertices;
	delete this->resourceIndices;
}

Resource* Mesh::GetResourceVertices() const
{
	return this->resourceVertices;
}

const size_t Mesh::GetSizeOfVertices() const
{
	return this->vertices.size() * sizeof(Vertex);
}

const size_t Mesh::GetNumVertices() const
{
	return this->vertices.size();
}

Resource* Mesh::GetResourceIndices() const
{
	return this->resourceIndices;
}

const size_t Mesh::GetSizeOfIndices() const
{
	return this->indices.size() * sizeof(unsigned int);
}

const size_t Mesh::GetNumIndices() const
{
	return this->indices.size();
}

const D3D12_INDEX_BUFFER_VIEW* Mesh::GetIndexBufferView() const
{
	return &this->indexBufferView;
}

const UINT Mesh::GetDescriptorHeapIndex() const
{
	return this->descriptorHeapIndex;
}

void Mesh::CreateIndexBufferView()
{
	this->indexBufferView.BufferLocation = this->resourceIndices->GetGPUVirtualAdress();
	this->indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	this->indexBufferView.SizeInBytes = this->GetSizeOfIndices();
}

