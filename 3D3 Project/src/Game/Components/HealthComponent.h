#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "Component.h"

namespace component
{
	class HealthComponent : public Component
	{
	public:
		HealthComponent();
		~HealthComponent();

		int GetHealth() const;

		void Update();
	private:
		int health = 0;
	};
}

#endif