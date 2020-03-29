#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "Mesh.h"

enum DrawOptions
{
    ForwardRendering = 1,
    Blend = 2,
    // Shadow = 4,
    // Bloom = 8,
    // etc..
};

class Object
{
public:
    Object(Mesh* mesh, UINT drawFlag);
    virtual ~Object();

    Transform* GetTransform();
    Mesh* GetMesh();
    UINT GetIndex();

    virtual void Init() = 0;
    void Update(double dt);
    virtual void UpdateSpecific(double dt) = 0;

    SlotInfo* GetSlotInfo();
    UINT GetDrawFlag();

protected:
    SlotInfo info;
    UINT index;

    Mesh* mesh;
 // Material* material
    Transform* transform;

private:
    UINT drawFlag = 0;
    
};

#endif