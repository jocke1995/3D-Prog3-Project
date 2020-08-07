#ifndef POINTLIGHTCOMPONENT_H
#define POINTLIGHTCOMPONENT_H

#include "Light.h"

namespace component
{
	class PointLightComponent : public Light, public Component
	{
	public:
		PointLightComponent(Entity* parent, unsigned int lightFlags = 0);
		virtual ~PointLightComponent();

		void SetPosition(float3 position);
		void SetAttenuation(float3 attenuation);

		void* GetLightData() const;

		void Update();
	private:
		PointLight* pointLight = nullptr;

		void UpdateLightData(LIGHT_COLOR_TYPE type);
	};
}

#endif