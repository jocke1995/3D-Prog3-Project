#ifndef MINOTAUR_H
#define MINOTAUR_H

#include "Engine/Object.h"


class Minotaur : public Object
{
public:
    Minotaur(Mesh* mesh, unsigned int drawFlag);
    ~Minotaur();
private:
    void Init();
    void UpdateSpecific(double dt);

};

#endif