#include "PointLightComponent.h"

namespace component
{
	PointLightComponent::PointLightComponent(Entity* parent, unsigned int lightFlags)
		:Component(parent), Light(lightFlags)
	{
		this->pointLight = new PointLight();
		this->pointLight->position = { 0.0f,  5.0f,  0.0f, 0.0f };
		this->pointLight->attenuation = { 1.0f, 0.09f, 0.032f, 0.0f };
		this->pointLight->baseLight = *this->baseLight;
	}

	PointLightComponent::~PointLightComponent()
	{
		delete this->pointLight;
	}

	void PointLightComponent::SetPosition(float3 position)
	{
		this->pointLight->position = { position.x, position.y, position.z, 1.0f };
	}

	void PointLightComponent::SetAttenuation(float3 attenuation)
	{
		this->pointLight->attenuation.x = attenuation.x;
		this->pointLight->attenuation.y = attenuation.y;
		this->pointLight->attenuation.z = attenuation.z;
	}

	PointLight* PointLightComponent::GetPointLight() const
	{
		return this->pointLight;
	}

	void PointLightComponent::Update()
	{
		if (this->lightFlags & LIGHT_FLAG::USE_TRANSFORM_POSITION)
		{
			Transform* tc = this->parent->GetComponent<TransformComponent>()->GetTransform();
			float3 position = tc->GetPositionFloat3();
			this->pointLight->position.x = position.x;
			this->pointLight->position.y = position.y;
			this->pointLight->position.z = position.z;
		}
	}

	void PointLightComponent::UpdateLightData(LIGHT_COLOR_TYPE type)
	{
		switch (type)
		{
		case LIGHT_COLOR_TYPE::LIGHT_AMBIENT:
			this->pointLight->baseLight.ambient = this->baseLight->ambient;
			break;
		case LIGHT_COLOR_TYPE::LIGHT_DIFFUSE:
			this->pointLight->baseLight.diffuse = this->baseLight->diffuse;
			break;
		case LIGHT_COLOR_TYPE::LIGHT_SPECULAR:
			this->pointLight->baseLight.specular = this->baseLight->specular;
			break;
		}
	}
}