#include "Cube.h"

Cube::Cube(Mesh* mesh, UINT index) : Object(mesh, index)
{
}

Cube::~Cube()
{
}

void Cube::Init()
{
	
}

void Cube::UpdateSpecific()
{
	static float s = 1;
	s += 0.01f;
	this->transform->SetScale(abs(sinf(s)), 1, 1);
}
