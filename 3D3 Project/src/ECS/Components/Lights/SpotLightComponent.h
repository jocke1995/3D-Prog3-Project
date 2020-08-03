#ifndef SPOTLIGHTCOMPONENT_H
#define SPOTLIGHTCOMPONENT_H

#include "Light.h"
namespace component
{
	class SpotLightComponent : public Light, public Component
	{
	public:
		SpotLightComponent(Entity* parent, unsigned int lightFlags = 0);
		virtual ~SpotLightComponent();

		void SetPosition(float3 position);
		void SetCutOff(float cutOff);
		void SetDirection(float3 direction);
		void SetOuterCutOff(float outerCutOff);
		void SetAttenuation(float3 attenuation);

		SpotLight* GetSpotLight() const;

		void Update();
	private:
		SpotLight* spotLight = nullptr;

		void UpdateLightData(LIGHT_COLOR_TYPE type);
	};
}


#endif