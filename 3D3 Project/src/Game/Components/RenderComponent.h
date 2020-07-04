#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "../../Engine/Transform.h"
#include "../../Engine/Mesh.h"
#include "Component.h"

namespace component
{

    class RenderComponent : public Component
    {
    public:
        RenderComponent();
        virtual ~RenderComponent();

        void Update();

        // Sets
        void SetMeshes(std::vector<Mesh*>* meshes);
        void SetDrawFlag(UINT drawFlag);

        // Gets
        Transform* GetTransform() const;
        Mesh* GetMesh(unsigned int index) const;
        UINT GetDrawFlag() const;
        unsigned int GetNrOfMeshes() const;
        SlotInfo* GetSlotInfo(unsigned int index) const;

    private:
        Transform* transform;
        std::vector<Mesh*> meshes;
        UINT drawFlag = 0;

        // Material* material
        std::vector<SlotInfo*> info;
    };
}

#endif