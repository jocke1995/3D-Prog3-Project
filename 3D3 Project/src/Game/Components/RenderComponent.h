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
    void SetMeshes(std::vector<Mesh*> *meshes);
    void SetDrawFlag(UINT drawFlag);

    // Gets
    Transform* GetTransform();
    Mesh* GetMesh(unsigned int index);
    UINT GetDrawFlag();
    unsigned int GetNrOfMeshes();
    SlotInfo* GetSlotInfo(unsigned int index);

private:
    Transform* transform;
    std::vector<Mesh*> meshes;
    UINT drawFlag = 0;

    // Material* material
    std::vector<SlotInfo*> info;
};

#endif