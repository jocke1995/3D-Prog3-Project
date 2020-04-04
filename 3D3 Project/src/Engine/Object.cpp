#include "Object.h"

Object::Object(Mesh* mesh, UINT drawFlag)
{
	this->index = index;
	this->info.vertexDataIndex = mesh->GetVertexDataIndex();
	this->transform = new Transform();
	this->mesh = mesh;

	this->drawFlag = drawFlag;
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

void Object::Update(double dt)
{
	this->transform->UpdateWorldMatrix();

	this->UpdateSpecific(dt);
}

SlotInfo* Object::GetSlotInfo()
{
	return &this->info;
}

UINT Object::GetDrawFlag()
{
	return this->drawFlag;
}

