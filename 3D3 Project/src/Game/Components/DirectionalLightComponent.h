#ifndef DIRECTIONALLIGHTCOMPONENT_H
#define DIRECTIONALLIGHTCOMPONENT_H

#include "Component.h"
#include "../../Engine/Resource.h"
#include "../Entity.h"
#include "RenderComponent.h"

enum LIGHT_FLAG
{
	USE_MESH_POSITION = BIT(1)
};

static int staticLightDescriptorHeapIndex = 0;
namespace component
{
	class DirectionalLightComponent : public Component
	{
	public:
		DirectionalLightComponent(Entity* parent);
		~DirectionalLightComponent();
		
		void CreateResource(ID3D12Device5* device);
		void SetLightFlag(unsigned int lightFlag);
		void SetPosition(float3 position);
		void SetColor(float4 color);

		unsigned int GetDescriptorHeapIndex() const;
		unsigned int GetCbSizeAligned() const;
		Resource* GetResource() const;


		void Update();
	private:
		CB_DirectionalLight* directionalLightStruct = nullptr;
		unsigned int lightFlag = 0;
		
		unsigned int lightDescriptorHeapIndex = -1;
		unsigned int cbSizeAligned = 0;
		Resource* resource = nullptr;
	};
}



#endif