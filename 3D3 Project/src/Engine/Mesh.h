#ifndef MESH_H
#define MESH_H

#include "stdafx.h"
#include "Resource.h"
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

    Mesh(ID3D12Device5* device, std::vector<Vertex> vertices, UINT size, UINT vertexDataIndex);
    ~Mesh();

    std::vector<Vertex> vertices;

    Resource* GetResource();

    void SetSize(size_t size);
    size_t GetSize();
    size_t GetNumVertices();
    UINT GetVertexDataIndex();

    void SetData();

    // Indici buffer

private:
    UINT vertexDataIndex;

    Resource* resource = nullptr;
};

#endif