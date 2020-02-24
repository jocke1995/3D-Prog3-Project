#include "Object.h"

Object::~Object()
{
}

Transform& Object::GetTransform()
{
	return this->transform;
}
