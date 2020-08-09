#include "SpotLightComponent.h"

namespace component
{
    SpotLightComponent::SpotLightComponent(Entity* parent, unsigned int lightFlags)
        :Component(parent), Light(CAMERA_TYPE::PERSPECTIVE, lightFlags)
    {
        
        this->spotLight = new SpotLight();
        this->spotLight->position_cutOff = { 0.0f, 0.0f, 0.0f, cos(XMConvertToRadians(12.5f)) };
        this->spotLight->direction_outerCutoff = { 0.0f, 0.0f, 0.0f, cos(XMConvertToRadians(17.5f)) };
        //this->spotLight->attenuation = { 1.0f, 0.14f, 0.07f, 0.0f };    // 32 'distance'
        this->spotLight->attenuation = { 1.0f, 0.09f, 0.032f, 0.0f }; 

        this->spotLight->baseLight = *this->baseLight;
    }

    SpotLightComponent::~SpotLightComponent()
    {
        delete this->spotLight;
    }

    void SpotLightComponent::SetPosition(float3 position)
    {
        this->spotLight->position_cutOff.x = position.x;
        this->spotLight->position_cutOff.y = position.y;
        this->spotLight->position_cutOff.z = position.z;
    }

    void SpotLightComponent::SetCutOff(float cutOff)
    {
        this->spotLight->position_cutOff.w = cutOff;
    }

    void SpotLightComponent::SetDirection(float3 direction)
    {
        this->spotLight->direction_outerCutoff.x = direction.x;
        this->spotLight->direction_outerCutoff.y = direction.y;
        this->spotLight->direction_outerCutoff.z = direction.z;
    }

    void SpotLightComponent::SetOuterCutOff(float outerCutOff)
    {
        this->spotLight->direction_outerCutoff.w = outerCutOff;
    }

    void SpotLightComponent::SetAttenuation(float3 attenuation)
    {
        this->spotLight->attenuation.x = attenuation.x;
        this->spotLight->attenuation.y = attenuation.y;
        this->spotLight->attenuation.z = attenuation.z;
    }

    void* SpotLightComponent::GetLightData() const
    {
        return this->spotLight;
    }

    void SpotLightComponent::Update()
    {
        if (this->lightFlags & LIGHT_FLAG::USE_TRANSFORM_POSITION)
        {
        	Transform* tc = this->parent->GetComponent<TransformComponent>()->GetTransform();
        	float3 position = tc->GetPositionFloat3();
            this->spotLight->position_cutOff.x = position.x;
            this->spotLight->position_cutOff.y = position.y;
            this->spotLight->position_cutOff.z = position.z;
        }
    }
    void SpotLightComponent::UpdateLightData(LIGHT_COLOR_TYPE type)
    {
        switch (type)
        {
        case LIGHT_COLOR_TYPE::LIGHT_AMBIENT:
            this->spotLight->baseLight.ambient = this->baseLight->ambient;
            break;
        case LIGHT_COLOR_TYPE::LIGHT_DIFFUSE:
            this->spotLight->baseLight.diffuse = this->baseLight->diffuse;
            break;
        case LIGHT_COLOR_TYPE::LIGHT_SPECULAR:
            this->spotLight->baseLight.specular = this->baseLight->specular;
            break;
        }
    }
}