#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "Mesh.h"

class Object
{
public:
    Object(ConstantBuffer* constantBuffer) { this->transform = Transform(constantBuffer); };
    virtual ~Object();

    Transform& GetTransform();

    virtual void Init() = 0;
    virtual void Update() = 0;

protected:
    
    std::vector<Mesh*> meshes;
    // TODO: Material*
    Transform transform;
    
};

#endif