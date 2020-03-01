#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	SAFE_RELEASE(&this->vertexBufferResource);
}

ID3D12Resource1** Mesh::GetVBResource()
{
	return &this->vertexBufferResource;
}

void Mesh::SetSize(size_t size)
{
	this->sizeOfVertexBuffer = size;
}

size_t Mesh::GetSize()
{
	return this->sizeOfVertexBuffer;
}

size_t Mesh::GetNumVertices()
{
	return this->sizeOfVertexBuffer / sizeof(Vertex);
}

void Mesh::SetData()
{
	void* dataBegin = nullptr;

	// Set up the heap data
	D3D12_RANGE range = { 0, 0 }; // We do not intend to read this resource on the CPU.

	this->vertexBufferResource->Map(0, &range, &dataBegin); // Get a dataBegin pointer where we can copy data to
	memcpy(dataBegin, this->vertices.data(), this->sizeOfVertexBuffer);
	this->vertexBufferResource->Unmap(0, nullptr);
}

