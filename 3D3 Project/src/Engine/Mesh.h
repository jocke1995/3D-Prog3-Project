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
        DirectX::XMFLOAT4 normal;
        DirectX::XMFLOAT4 tangent;
    };

    Mesh(ID3D12Device5* device, std::vector<Vertex> vertices, UINT vertexDataIndex);
    ~Mesh();

    std::vector<Vertex> vertices;

    Resource* GetResource() const;
    const size_t GetSize() const;
    const size_t GetNumVertices() const;
    const UINT GetVertexDataIndex() const;

private:
    UINT vertexDataIndex;

    Resource* resource = nullptr;
};

#endif