#include "Cube.h"

Cube::Cube(Mesh* mesh) : Object(mesh)
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
	this->transform->SetScale(1, 1, 1);
}
