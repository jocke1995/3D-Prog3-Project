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

    virtual void Init() = 0;
    void Update();
    virtual void UpdateSpecific() = 0;

    SlotInfo* GetSlotInfo();

protected:
    SlotInfo info;

    Mesh* mesh;
    // TODO: Material*
    Transform* transform;
    
};

#endif