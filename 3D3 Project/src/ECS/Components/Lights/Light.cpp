#include "Light.h"

Light::Light(unsigned int lightFlags)
{
	this->lightFlags = lightFlags;

	this->baseLight = new BaseLight();
	this->baseLight->ambient = { 0.15f, 0.15f, 0.15f, 1.0f };
	this->baseLight->diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	this->baseLight->specular = { 0.1f, 0.1f, 0.1f, 1.0f };
}

Light::~Light()
{
	delete this->baseLight;
}

void Light::SetColor(LIGHT_COLOR_TYPE type, float4 color)
{
	switch (type)
	{
	case LIGHT_COLOR_TYPE::LIGHT_AMBIENT:
		this->baseLight->ambient = color;
		break;
	case LIGHT_COLOR_TYPE::LIGHT_DIFFUSE:
		this->baseLight->diffuse = color;
		break;
	case LIGHT_COLOR_TYPE::LIGHT_SPECULAR:
		this->baseLight->specular = color;
		break;
	}

	this->UpdateLightData(type);
}

void Light::SetShadowInfo(ShadowInfo* shadowInfo)
{
	this->shadowInfo = shadowInfo;
}

unsigned int Light::GetLightFlags() const
{
	return this->lightFlags;
}

ShadowInfo* Light::GetShadowInfo() const
{
	return this->shadowInfo;
}
