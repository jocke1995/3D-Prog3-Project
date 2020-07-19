#include "DirectionalLightComponent.h"

namespace component
{
	DirectionalLightComponent::DirectionalLightComponent(Entity* parent)
		:Component(parent)
	{
		this->lightDescriptorHeapIndex = 10;// staticLightDescriptorHeapIndex++;

		this->directionalLightStruct = new CB_DirectionalLight();
		this->directionalLightStruct->position = { 0.0f, 0.0f, 0.0f, 0.0f }; 
		this->directionalLightStruct->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	DirectionalLightComponent::~DirectionalLightComponent()
	{
		delete this->directionalLightStruct;
		delete this->resource;
	}

	unsigned int DirectionalLightComponent::GetDescriptorHeapIndex() const
	{
		return this->lightDescriptorHeapIndex;
	}

	unsigned int DirectionalLightComponent::GetCbSizeAligned() const
	{
		return this->cbSizeAligned;
	}

	Resource* DirectionalLightComponent::GetResource() const
	{
		return this->resource;
	}

	void DirectionalLightComponent::CreateResource(ID3D12Device5* device)
	{
		this->cbSizeAligned = (sizeof(CB_DirectionalLight) + 255) & ~255;
		this->resource = new Resource(device, this->cbSizeAligned, RESOURCE_TYPE::UPLOAD, L"DirectionalLight");
		this->resource->SetData(this->directionalLightStruct);
	}

	void DirectionalLightComponent::SetLightFlag(unsigned int lightFlag)
	{
		this->lightFlag = lightFlag;
	}

	void DirectionalLightComponent::SetPosition(float3 position)
	{
		this->directionalLightStruct->position = { position.x, position.y, position.z, 1.0 };
	}

	void DirectionalLightComponent::SetColor(float4 color)
	{
		this->directionalLightStruct->color = color;
	}

	void DirectionalLightComponent::Update()
	{
		if (this->lightFlag & LIGHT_FLAG::USE_MESH_POSITION)
		{
			Transform* tc = this->parent->GetComponent<TransformComponent>()->GetTransform();
			float3 position = tc->GetPositionFloat3();
			this->directionalLightStruct->position = { position.x, position.y, position.z, 0.0 };
		}
		//static float g = 0;
		//g += 0.0025f;
		//
		//this->directionalLightStruct->color.g = g;
		//this->directionalLightStruct->color.g = abs(sin(this->directionalLightStruct->color.g));

		this->resource->SetData(this->directionalLightStruct);
	}
}
