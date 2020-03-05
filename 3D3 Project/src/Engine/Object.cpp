#include "Object.h"

Object::Object(Mesh* mesh, UINT index)
{
	this->index = index;
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

UINT Object::GetIndex()
{
	return this->index;
}

void Object::Update()
{
	this->transform->UpdateWorldMatrix();

	this->UpdateSpecific();
}

SlotInfo* Object::GetSlotInfo()
{
	return &this->info;
}
