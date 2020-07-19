#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../../Engine/Transform.h"
#include "Component.h"

namespace component
{
    class TransformComponent : public Component
    {
    public:
        TransformComponent(Entity* parent);
        virtual ~TransformComponent();

        void Update();

        Transform* GetTransform() const;
    private:
        Transform* transform = nullptr;
    };
}

#endif

