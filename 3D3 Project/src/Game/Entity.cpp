#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
	for (Component* component : this->components)
	{
		delete component;
	}
}

void Entity::Update()
{
	for (Component* component : this->components)
	{
		component->Update();
	}
}
