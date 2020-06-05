#ifndef ENTITY_H
#define ENTITY_H

#include "Components/HealthComponent.h"
#include "Components/RenderComponent.h"
#include "../Engine/stdafx.h"
class Component;

class Entity
{
public:
	Entity();
	~Entity();

	template <class T>
	void AddComponent();

	template <class T>
	T* GetComponent();


	void Update();

private:
	std::vector<Component*> components;
};

template<class T>
void Entity::AddComponent()
{
	this->components.push_back(new T);
}

template<class T>
T* Entity::GetComponent()
{
	for (int i = 0; i < this->components.size(); i++)
	{
		T* component = dynamic_cast<T*>(this->components[i]);

		if (component != nullptr)
		{
			return component;
		}
	}
	return nullptr;
}

#endif // !ENTITY_H