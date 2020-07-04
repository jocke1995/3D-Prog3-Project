#include "HealthComponent.h"

namespace component
{
	HealthComponent::HealthComponent()
	{
		this->health = 100;
	}

	HealthComponent::~HealthComponent()
	{
	}

	int HealthComponent::GetHealth() const
	{
		return this->health;
	}

	void HealthComponent::Update()
	{

	}
}

