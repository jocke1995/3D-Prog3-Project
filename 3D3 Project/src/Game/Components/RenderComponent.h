#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "../../Engine/Transform.h"
#include "../../Engine/Mesh.h"
#include "Component.h"

class RenderComponent : public Component
{
public:
    RenderComponent();
    virtual ~RenderComponent();

    void Update();

    // Sets
    void SetMesh(Mesh* mesh);
    void SetDrawFlag(UINT drawFlag);

    // Gets
    Transform* GetTransform();
    Mesh* GetMesh();
    UINT GetDrawFlag();
    SlotInfo* GetSlotInfo();

private:
    Transform* transform;
    Mesh* mesh;
    UINT drawFlag = 0;

    // Material* material
    SlotInfo info;
};

#endif