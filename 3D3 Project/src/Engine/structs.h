#pragma once

// Indicies of where the data is stored in GPU-memory
struct SlotInfo
{
	unsigned int vertexDataIndex;
	float3 padding;
	// MaterialIndex
	// Texture_Diffuse;
	// TextureNormal:
	// Texture Specular;
};

struct CB_PER_ENTITY
{
	float4x4 worldMatrix;
	float4x4 WVP;
	SlotInfo info;
};

struct CB_PER_FRAME
{
	float3 camPos;
	// deltaTime ..
	// etc ..
};

struct PointLight
{
	float4 position;
	float4 color;
};