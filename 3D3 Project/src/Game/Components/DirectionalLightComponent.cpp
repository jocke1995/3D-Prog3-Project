#include "DirectionalLightComponent.h"

namespace component
{
	DirectionalLightComponent::DirectionalLightComponent(Entity* parent)
		:Component(parent)
	{
		this->lightDescriptorHeapIndex = staticLightDescriptorHeapIndex++;

		this->directionalLightStruct = new CB_DirectionalLight();
		this->directionalLightStruct->position = { 0.0f, 0.0f, 0.0f, 0.0f }; 
		this->directionalLightStruct->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	DirectionalLightComponent::~DirectionalLightComponent()
	{
		delete this->directionalLightStruct;
		delete this->resource;
	}

	void DirectionalLightComponent::SetDirectionalLight(CB_DirectionalLight* dlData)
	{
		this->directionalLightStruct->position = dlData->position;
		this->directionalLightStruct->color = dlData->color;
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

	void DirectionalLightComponent::Update()
	{
		static float g = 0;
		g += 0.0025f;

		this->directionalLightStruct->color.g = g;
		this->directionalLightStruct->color.g = abs(sin(this->directionalLightStruct->color.g));

		this->resource->SetData(this->directionalLightStruct);
	}
}
