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

void Cube::UpdateSpecific(double dt)
{
	static float s = 1;
	s += 1.0f * dt;
	this->transform->SetScale(abs(sinf(s)), 1, 1);
}
