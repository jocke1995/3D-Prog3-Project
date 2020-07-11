#ifndef DIRECTIONALLIGHTCOMPONENT_H
#define DIRECTIONALLIGHTCOMPONENT_H

#include "Component.h"
#include "../../Engine/Resource.h"

static int staticLightDescriptorHeapIndex = 0;
namespace component
{
	class DirectionalLightComponent : public Component
	{
	public:
		DirectionalLightComponent(Entity* parent);
		~DirectionalLightComponent();
		
		void CreateResource(ID3D12Device5* device);
		void SetDirectionalLight(CB_DirectionalLight* dlData);

		unsigned int GetDescriptorHeapIndex() const;
		unsigned int GetCbSizeAligned() const;
		Resource* GetResource() const;


		void Update();
	private:
		CB_DirectionalLight* directionalLightStruct = nullptr;
		
		unsigned int lightDescriptorHeapIndex = -1;
		unsigned int cbSizeAligned = 0;
		Resource* resource = nullptr;
	};
}



#endif