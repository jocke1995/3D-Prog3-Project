#ifndef LIGHT_H
#define LIGHT_H

#include "../../Entity.h"
#include "../TransformComponent.h"
#include "../Component.h"
#include "../src/Engine/stdafx.h"
#include "../src/Engine/structs.h"

#include "../src/Engine/OrthographicCamera.h"
#include "../src/Engine/PerspectiveCamera.h"

enum LIGHT_FLAG
{
	// Set flag to make the light position inherit the position of the corresponding mesh
	USE_TRANSFORM_POSITION = BIT(1),

	// Option to make the light cast shadows or not.
	CAST_SHADOW = BIT(2)

	// If this is set, renderer only need to copy data once to GPU
	// StaticSettings.. = BIT(3),

	// etc..
};

enum LIGHT_TYPE
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
	NUM_LIGHT_TYPES
};

enum LIGHT_COLOR_TYPE
{
	LIGHT_AMBIENT,
	LIGHT_DIFFUSE,
	LIGHT_SPECULAR,
	NUM_COLOR_TYPES
};

class Light
{
public:
	Light(CAMERA_TYPE camType, unsigned int lightFlags = 0);
	virtual ~Light();

	void SetColor(LIGHT_COLOR_TYPE type, float4 color);

	// Gets
	unsigned int GetLightFlags() const;
	virtual void* GetLightData() const = 0;
	BaseCamera* GetCamera() const;

protected:
	BaseLight* baseLight = nullptr;
	virtual void UpdateLightData(LIGHT_COLOR_TYPE type) = 0;

	unsigned int lightFlags = 0;

	BaseCamera* camera = nullptr;
	CAMERA_TYPE cameraType;
private:

	void CreateCamera();
};

#endif