#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "Mesh.h"

// TODO: Nu f�r man �ndra manuellt n�r man g�r fler rendertasks... kom ih�g: Samma som en enum i stdafx.h
struct DrawOptions
{
    DrawOptions()
    {
        test = false;
        blend = false;
    }

    bool test;
    bool blend;
};

class Object
{
public:
    Object(Mesh* mesh, DrawOptions* drawOptions);
    virtual ~Object();

    Transform* GetTransform();
    Mesh* GetMesh();
    UINT GetIndex();

    virtual void Init() = 0;
    void Update(double dt);
    virtual void UpdateSpecific(double dt) = 0;

    SlotInfo* GetSlotInfo();
    DrawOptions* GetDrawOptions();

protected:
    SlotInfo info;
    UINT index;

    Mesh* mesh;
    // TODO: Material*
    Transform* transform;

private:
    DrawOptions *drawOptions;
    
};

#endif