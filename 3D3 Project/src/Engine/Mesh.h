#ifndef MESH_H
#define MESH_H

#include "stdafx.h"
#include <vector>

class Mesh
{
public:
    struct Vertex
    {
        DirectX::XMFLOAT4 pos;
        //DirectX::XMFLOAT4 uv;
        //DirectX::XMFLOAT4 nor;
    };

    Mesh(ID3D12Device5* device, std::vector<Vertex> vertices, UINT size);
    ~Mesh();


    std::vector<Vertex> vertices;

    ID3D12Resource1* GetVBResource();

    void SetSize(size_t size);
    size_t GetSize();
    size_t GetNumVertices();

    void SetData();

    // Indici buffer

    // TODO: SRV

private:
    ID3D12Resource1* vertexBufferResource = nullptr;

    size_t sizeOfVertexBuffer;

    // TODO: Används ej just nu, kanske ska göra det senare
    D3D12_HEAP_PROPERTIES hp = {};
    D3D12_RESOURCE_DESC rd = {};
};

#endif