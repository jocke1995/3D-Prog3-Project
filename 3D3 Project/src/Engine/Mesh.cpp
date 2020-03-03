#include "Mesh.h"

Mesh::Mesh(ID3D12Device5* device, std::vector<Vertex> vertices, UINT size)
{
	this->sizeOfVertexBuffer = size;
	this->vertices = vertices;

	// TODO: Use default heap aswell?
	D3D12_HEAP_PROPERTIES hp = {};
	hp.Type = D3D12_HEAP_TYPE_UPLOAD;
	hp.CreationNodeMask = 1;
	hp.VisibleNodeMask = 1;

	
	D3D12_RESOURCE_DESC rd = {};
	rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	rd.Width = this->sizeOfVertexBuffer; // numVertices * Sizeof(vertices)
	rd.Height = 1;
	rd.DepthOrArraySize = 1;
	rd.MipLevels = 1;
	rd.SampleDesc.Count = 1;
	rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr = device->CreateCommittedResource(
		&hp,
		D3D12_HEAP_FLAG_NONE,
		&rd,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->vertexBufferResource));

	if (FAILED(hr))
	{
		OutputDebugStringW(L"ERROR: Failed to create Vertex Buffer\n");
	}

	this->vertexBufferResource->SetName(L"VB_Mesh");

	this->SetData();
}

Mesh::~Mesh()
{
	SAFE_RELEASE(&this->vertexBufferResource);
}

ID3D12Resource1* Mesh::GetVBResource()
{
	return this->vertexBufferResource;
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

