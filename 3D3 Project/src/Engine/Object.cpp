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
	this->transform->UpdateWorldMatrix();

	this->UpdateSpecific();
}
