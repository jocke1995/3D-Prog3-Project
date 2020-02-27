#ifndef MESH_H
#define MESH_H

#include "stdafx.h"
#include <vector>

class Mesh
{
public:
    Mesh() {};
    ~Mesh() {};

    struct Vertex
    {
        DirectX::XMFLOAT4 pos;
        DirectX::XMFLOAT4 uv;
        DirectX::XMFLOAT4 nor;
    };

    std::vector<Vertex> vertices;

    // Indici buffer

    // TODO: SRV

private:
    


};

#endif