#include "Entity.h"

Entity::Entity()
{
	this->id = staticID++;
}

bool Entity::operator==(const Entity* rhs) const
{
	if (this->id == rhs->id)
	{
		return true;
	}
	return false;
}

Entity::~Entity()
{
	for (Component* component : this->components)
	{
		delete component;
	}
}

unsigned int Entity::GetID() const
{
	return this->id;
}

void Entity::Update()
{
	for (Component* component : this->components)
	{
		component->Update();
	}
}
