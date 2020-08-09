#include "Light.h"

Light::Light(CAMERA_TYPE camType, unsigned int lightFlags)
{
	this->cameraType = camType;
	this->CreateCamera();

	this->lightFlags = lightFlags;

	this->baseLight = new BaseLight();
	this->baseLight->ambient = { 0.15f, 0.15f, 0.15f, 1.0f };
	this->baseLight->diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	this->baseLight->specular = { 0.1f, 0.1f, 0.1f, 1.0f };
}

Light::~Light()
{
	delete this->baseLight;

	delete this->camera;
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


unsigned int Light::GetLightFlags() const
{
	return this->lightFlags;
}

BaseCamera* Light::GetCamera() const
{
	return this->camera;
}

void Light::CreateCamera()
{
	switch (this->cameraType)
	{
	case CAMERA_TYPE::ORTHOGRAPHIC:
		this->camera = new OrthographicCamera();
		break;
	}
}
