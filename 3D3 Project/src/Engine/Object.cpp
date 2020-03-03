#include "Object.h"

Object::Object(ConstantBuffer* transformBuffer, Mesh* mesh)
{
	this->info.vertexDataIndex = mesh->GetVertexDataIndex();
	this->transform = new Transform(transformBuffer);
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
