#ifndef ENTITY_H
#define ENTITY_H

//#include "Components/RenderComponent.h"
//#include "Components/DirectionalLightComponent.h"

class RenderComponent;
class DirectionalLightComponent;

#include "../Engine/stdafx.h"
#include "Components/Component.h"

static unsigned int staticID = 0;
class Entity
{
public:
	Entity();
	bool operator == (const Entity* rhs) const;
	~Entity();

	template <class T>
	void AddComponent();

	template <class T>
	T* GetComponent() const;

	unsigned int GetID() const;

	void Update();

private:
	unsigned int id = -1;

	std::vector<Component*> components;
};

template<class T>
void Entity::AddComponent()
{
	this->components.push_back(new T(this));
}

template<class T>
T* Entity::GetComponent() const
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