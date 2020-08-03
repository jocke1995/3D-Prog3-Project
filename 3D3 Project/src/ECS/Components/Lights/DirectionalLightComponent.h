#ifndef DIRECTIONALLIGHTCOMPONENT_H
#define DIRECTIONALLIGHTCOMPONENT_H

#include "Light.h"

namespace component
{
	class DirectionalLightComponent :  public Light, public Component
	{
	public:
		DirectionalLightComponent(Entity* parent, unsigned int lightFlags = 0);
		virtual ~DirectionalLightComponent();

		
		//void SetPosition(float3 position);
		void SetDirection(float3 direction);

		DirectionalLight* GetDirectionalLight() const;

		void Update();
	private:
		DirectionalLight* directionalLight = nullptr;

		void UpdateLightData(LIGHT_COLOR_TYPE type);
	};
}

#endif