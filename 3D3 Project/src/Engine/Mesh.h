#ifndef MESH_H
#define MESH_H

#include "Resource.h"
#include "Texture.h"

class Mesh
{
public:
    struct Vertex
    {
        DirectX::XMFLOAT4 pos;
        DirectX::XMFLOAT4 uv;
        DirectX::XMFLOAT4 normal;
        DirectX::XMFLOAT4 tangent;
    };

    Mesh(   ID3D12Device5* device,
            std::vector<Vertex> vertices,
            std::vector<unsigned int> indices,
            unsigned int descriptorHeapIndex_SRV);
    Mesh(const Mesh* other);
    ~Mesh();

    void UploadToDefault(ID3D12Device5* device, CommandInterface* commandInterface, ID3D12CommandQueue* cmdQueue);

    // Sets
    void SetTexture(TEXTURE_TYPE textureType, Texture* texture);

    // Gets
    Resource* GetDefaultResourceVertices() const;
    const size_t GetSizeOfVertices() const;
    const size_t GetNumVertices() const;

    Resource* GetDefaultResourceIndices() const;
    const size_t GetSizeOfIndices() const;
    const size_t GetNumIndices() const;
    const D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() const;

    const SlotInfo* GetSlotInfo() const;
    const UINT GetDescriptorHeapIndex() const;

    Texture* GetTexture(TEXTURE_TYPE textureType);

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    SlotInfo* slotInfo = nullptr;
    UINT descriptorHeapIndex_SRV;

    std::map<TEXTURE_TYPE,Texture*> textures;

    Resource* uploadResourceVertices = nullptr;
    Resource* uploadResourceIndices = nullptr;
    Resource* defaultResourceVertices = nullptr;
    Resource* defaultResourceIndices = nullptr;

    D3D12_INDEX_BUFFER_VIEW indexBufferView = {};
    void CreateIndexBufferView();

    // Temporay solution to make sure each "new" mesh only gets deleted once
    bool isCopied = false;
};

#endif