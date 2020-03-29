#include "Minotaur.h"

Minotaur::Minotaur(Mesh* mesh, unsigned int drawFlag) : Object(mesh, drawFlag)
{
}

Minotaur::~Minotaur()
{
}

void Minotaur::Init()
{
	
}

void Minotaur::UpdateSpecific(double dt)
{
	// Test Rotation
	static float r = 1;
	r += 1.0f * dt;
	//this->transform->RotateX(r);
	this->transform->RotateY(r);
	//this->transform->RotateZ(r);

	this->transform->SetScale(0.07);

	//// Test Translation
	//XMFLOAT3 pos = this->transform->GetPosition();
	//static float px = 0.0f;
	//px += 0.0005f;
	//this->transform->SetPosition(abs(sinf(px)), pos.y, pos.z);
}
