#ifndef CUBE_H
#define CUBE_H

#include "Object.h"


class Cube : public Object
{
public:
    Cube(ConstantBuffer* constantBuffer, Mesh* mesh);
    ~Cube();

private:
    void Init();
    void Update();


};

#endif