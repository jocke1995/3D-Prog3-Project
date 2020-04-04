#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "Mesh.h"

enum DrawOptions
{
    ForwardRendering = 0x01,
    Blend = 0x02,
    // Shadow = 0x04,
    // Bloom = 0x08,
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