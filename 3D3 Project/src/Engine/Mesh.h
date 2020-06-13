#ifndef MESH_H
#define MESH_H

#include "Resource.h"

class Mesh
{
public:
    struct Vertex
    {
        DirectX::XMFLOAT4 pos;
        DirectX::XMFLOAT4 uv;
        DirectX::XMFLOAT4 nor;
    };

    Mesh(ID3D12Device5* device, std::vector<Vertex> vertices, UINT vertexDataIndex);
    ~Mesh();

    std::vector<Vertex> vertices;

    Resource* GetResource();

    size_t GetSize();
    size_t GetNumVertices();
    UINT GetVertexDataIndex();

private:
    UINT vertexDataIndex;

    Resource* resource = nullptr;
};

#endif