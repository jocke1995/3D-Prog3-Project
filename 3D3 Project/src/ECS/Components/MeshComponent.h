#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "../../Engine/Mesh.h"
#include "Component.h"

namespace component
{
    class MeshComponent : public Component
    {
    public:
        MeshComponent(Entity* parent);
        virtual ~MeshComponent();

        void Update();

        // Sets
        void SetMeshes(std::vector<Mesh*>* meshes);
        void SetDrawFlag(UINT drawFlag);

        // Gets
        Mesh* GetMesh(unsigned int index) const;
        UINT GetDrawFlag() const;
        unsigned int GetNrOfMeshes() const;

    private:
        std::vector<Mesh*> meshes;
        UINT drawFlag = 0;
    };
}
#endif