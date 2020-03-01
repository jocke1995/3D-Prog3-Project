#include "Object.h"

Object::Object(ConstantBuffer* constantBuffer, Mesh* mesh)
{
	this->transform = new Transform(constantBuffer);
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
