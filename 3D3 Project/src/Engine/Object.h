#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "Mesh.h"

class Object
{
public:
    Object(Mesh* mesh);
    virtual ~Object();

    Transform* GetTransform();
    Mesh* GetMesh();
    UINT GetIndex();

    virtual void Init() = 0;
    void Update(double dt);
    virtual void UpdateSpecific(double dt) = 0;

    SlotInfo* GetSlotInfo();

protected:
    SlotInfo info;
    UINT index;

    Mesh* mesh;
    // TODO: Material*
    Transform* transform;
    
};

#endif