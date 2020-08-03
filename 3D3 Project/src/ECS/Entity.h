#ifndef ENTITY_H
#define ENTITY_H

class MeshComponent;
class TransformComponent;
class DirectionalLightComponent;
class PointLightComponent;
class SpotLightComponent;

#include "../Engine/stdafx.h"
#include "Components/Component.h"

static unsigned int staticID = 0;
class Entity
{
public:
	Entity();
	bool operator == (const Entity* rhs) const;
	~Entity();

	template <class T, typename... Args>
	T* AddComponent(Args... args);

	template <class T>
	T* GetComponent() const;

	template <class T>
	bool HasComponent() const;

	unsigned int GetID() const;

	void Update();

private:
	unsigned int id = -1;

	std::vector<Component*> components;
};

template<class T, typename... Args>
inline T* Entity::AddComponent(Args... args)
{
	// Check if component already exists,
	// and if it does.. return it
	T* compInEntity = this->GetComponent<T>();
	if (compInEntity == nullptr)
	{
		// Add component
		T* finalComponent = new T(this, std::forward<Args>(args)...);
		this->components.push_back(finalComponent);

		return finalComponent;
	}
	return compInEntity;
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

template<class T>
inline bool Entity::HasComponent() const
{
	for (int i = 0; i < this->components.size(); i++)
	{
		T* component = dynamic_cast<T*>(this->components[i]);

		if (component != nullptr)
		{
			// Found
			return true;
		}
	}
	return false;
}

#endif // !ENTITY_H