#include "Object.h"

Object::~Object()
{
	delete this->transform;
}

Transform* Object::GetTransform()
{
	return this->transform;
}
