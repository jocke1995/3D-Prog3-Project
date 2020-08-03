#include "Mesh.h"

Mesh::Mesh(	ID3D12Device5* device,
			std::vector<Vertex> vertices,
			std::vector<unsigned int> indices,
			unsigned int descriptorHeapIndex_SRV)
{
	this->vertices = vertices;
	this->indices = indices;

	this->slotInfo = new SlotInfo();
	this->descriptorHeapIndex_SRV = descriptorHeapIndex_SRV;
	this->slotInfo->vertexDataIndex = this->descriptorHeapIndex_SRV;

	// Set vertices
	this->resourceVertices = new Resource(device, this->GetSizeOfVertices(), RESOURCE_TYPE::UPLOAD, L"Vertex Resource");
	this->resourceVertices->SetData(this->vertices.data());

	// Set indices
	this->resourceIndices = new Resource(device, this->GetSizeOfIndices(), RESOURCE_TYPE::UPLOAD, L"Index Resource");
	this->resourceIndices->SetData(this->indices.data());
	this->CreateIndexBufferView();
}

Mesh::Mesh(const Mesh* other)
{
	this->isCopied = true;

	// TODO: use the same vertices instead of copy
	this->vertices = other->vertices;
	this->indices = other->indices;

	this->slotInfo = new SlotInfo();
	this->slotInfo->vertexDataIndex = other->slotInfo->vertexDataIndex;
	this->slotInfo->textureAmbient  = other->slotInfo->textureAmbient;
	this->slotInfo->textureDiffuse  = other->slotInfo->textureDiffuse;
	this->slotInfo->textureSpecular = other->slotInfo->textureSpecular;
	this->slotInfo->textureNormal   = other->slotInfo->textureNormal;
	this->slotInfo->textureEmissive = other->slotInfo->textureEmissive;

	this->descriptorHeapIndex_SRV = other->descriptorHeapIndex_SRV;
	this->slotInfo->vertexDataIndex = other->descriptorHeapIndex_SRV;

	this->resourceVertices = other->resourceVertices;
	this->resourceIndices = other->resourceIndices;

	this->indexBufferView = other->indexBufferView;
}

Mesh::~Mesh()
{
	delete this->slotInfo;

	if (this->isCopied == false)
	{
		delete this->resourceVertices;
		delete this->resourceIndices;
	}
}

void Mesh::SetTexture(TEXTURE_TYPE textureType, Texture* texture)
{
	this->textures[textureType] = texture;
	
	switch (textureType)
	{
	case TEXTURE_TYPE::AMBIENT:
		this->slotInfo->textureAmbient = texture->GetDescriptorHeapIndex();
		break;
	case TEXTURE_TYPE::DIFFUSE:
		this->slotInfo->textureDiffuse = texture->GetDescriptorHeapIndex();
		break;
	case TEXTURE_TYPE::SPECULAR:
		this->slotInfo->textureSpecular = texture->GetDescriptorHeapIndex();
		break;
	case TEXTURE_TYPE::NORMAL:
		this->slotInfo->textureNormal = texture->GetDescriptorHeapIndex();
		break;
	case TEXTURE_TYPE::EMISSIVE:
		this->slotInfo->textureEmissive = texture->GetDescriptorHeapIndex();
		break;
	}
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

const SlotInfo* Mesh::GetSlotInfo() const
{
	return this->slotInfo;
}

const UINT Mesh::GetDescriptorHeapIndex() const
{
	return this->descriptorHeapIndex_SRV;
}

Texture* Mesh::GetTexture(TEXTURE_TYPE textureType)
{
	return this->textures[textureType];
}

void Mesh::CreateIndexBufferView()
{
	this->indexBufferView.BufferLocation = this->resourceIndices->GetGPUVirtualAdress();
	this->indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	this->indexBufferView.SizeInBytes = this->GetSizeOfIndices();
}

