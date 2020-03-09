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

void Cube::UpdateSpecific(double dt)
{
	// Test Rotation
	static float r = 1;
	r += 1.0f * dt;
	this->transform->RotateX(r);
	this->transform->RotateY(r);
	this->transform->RotateZ(r);

	//// Test Translation
	//XMFLOAT3 pos = this->transform->GetPosition();
	//static float px = 0.0f;
	//px += 0.0005f;
	//this->transform->SetPosition(abs(sinf(px)), pos.y, pos.z);
}
