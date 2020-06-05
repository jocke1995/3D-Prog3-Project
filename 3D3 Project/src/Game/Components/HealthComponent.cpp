#include "HealthComponent.h"

HealthComponent::HealthComponent()
{
	this->health = 100;
}

HealthComponent::~HealthComponent()
{
}

void HealthComponent::Update()
{

}

int HealthComponent::GetHealth()
{
	return this->health;
}
