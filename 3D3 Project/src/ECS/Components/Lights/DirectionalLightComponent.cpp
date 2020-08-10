#include "DirectionalLightComponent.h"

namespace component
{
	DirectionalLightComponent::DirectionalLightComponent(Entity* parent, unsigned int lightFlags)
		:Component(parent), Light(CAMERA_TYPE::ORTHOGRAPHIC, lightFlags)
	{
		this->directionalLight = new DirectionalLight();
		this->directionalLight->direction = { -1.0f,  -0.5f,  0.0f, 0.0f };
		this->directionalLight->baseLight = *this->baseLight;
	}

	DirectionalLightComponent::~DirectionalLightComponent()
	{
		delete this->directionalLight;
	}

	void DirectionalLightComponent::Init()
	{
		if (this->lightFlags & LIGHT_FLAG::CAST_SHADOW)
		{
			this->CreateCamera({ 
					-this->directionalLight->direction.x * 10,
					-this->directionalLight->direction.y * 10,
					-this->directionalLight->direction.z * 10},
					{this->directionalLight->direction.x, 
					this->directionalLight->direction.y, 
					this->directionalLight->direction.z, } );
		}
	}

	void DirectionalLightComponent::Update(double dt)
	{
		if (this->camera != nullptr)
		{
			this->camera->Update(dt);
		}
	}

	void DirectionalLightComponent::SetDirection(float3 direction)
	{
		this->directionalLight->direction = { direction.x, direction.y, direction.z, 0.0f };
		
		this->camera->SetPosition(-direction.x * 10, -direction.y * 10, -direction.z * 10);
		this->camera->SetLookAt(direction.x, direction.y, direction.z);
	}

	void* DirectionalLightComponent::GetLightData() const
	{
		return this->directionalLight;
	}

	void DirectionalLightComponent::UpdateLightData(LIGHT_COLOR_TYPE type)
	{
		switch (type)
		{
		case LIGHT_COLOR_TYPE::LIGHT_AMBIENT:
			this->directionalLight->baseLight.ambient = this->baseLight->ambient;
			break;
		case LIGHT_COLOR_TYPE::LIGHT_DIFFUSE:
			this->directionalLight->baseLight.diffuse = this->baseLight->diffuse;
			break;
		case LIGHT_COLOR_TYPE::LIGHT_SPECULAR:
			this->directionalLight->baseLight.specular = this->baseLight->specular;
			break;
		}
	}
}
