#include "Object.h"

Object::Object(Mesh* mesh)
{
	this->info.vertexDataIndex = mesh->GetVertexDataIndex();
	this->transform = new Transform();
	this->mesh = mesh;
}

Object::~Object()
{
	delete this->transform;
}

Transform* Object::GetTransform()
{
	return this->transform;
}

Mesh* Object::GetMesh()
{
	return this->mesh;
}

void Object::Update()
{
	static float s = 1;
	s += 0.01f;
	this->transform->SetScale(abs(sinf(s)), 1, 1);

	this->transform->UpdateWorldMatrix();

	this->UpdateSpecific();
}

SlotInfo* Object::GetSlotInfo()
{
	return &this->info;
}
