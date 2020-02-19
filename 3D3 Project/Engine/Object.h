#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "Mesh.h"

class Object
{
public:
    Object();
    ~Object();

    std::vector<Mesh*> meshes;
    // TODO: Material*
    Transform& transform;

private:
    virtual void Init() = 0;
    virtual void Update() = 0;


};

#endif