#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "Mesh.h"

class Object
{
public:
    struct SlotInfo
    {
        UINT vertexDataIndex = 0;
        // MaterialIndex
        // Texture_Diffuse;
        // TextureNormal:
        // Texture Specular;
    };

    Object(ConstantBuffer* constantBuffer, Mesh* mesh);
    virtual ~Object();

    Transform* GetTransform();
    Mesh* GetMesh();

    virtual void Init() = 0;
    virtual void Update() = 0;

protected:
    SlotInfo info;

    Mesh* mesh;
    // TODO: Material*
    Transform* transform;
    
};

#endif