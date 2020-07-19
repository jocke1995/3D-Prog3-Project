#ifndef ENTITY_H
#define ENTITY_H

class MeshComponent;
class TransformComponent;
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
	T* AddComponent();

	template <class T>
	T* GetComponent() const;

	unsigned int GetID() const;

	void Update();

private:
	unsigned int id = -1;

	std::vector<Component*> components;
};

template<class T>
inline T* Entity::AddComponent()
{
	// Check if component allready exists
	T* compInEntity = this->GetComponent<T>();
	if (compInEntity == nullptr)
	{
		// Add component
		T* finalComponent = new T(this);
		this->components.push_back(finalComponent);

		return finalComponent;
	}
	return nullptr;
}

template<class T>
inline T* Entity::GetComponent() const
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