#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "Component.h"
class HealthComponent : public Component
{
public:
	HealthComponent();
	~HealthComponent();

	void Update();
	int GetHealth();
private:
	int health = 0;
};

#endif