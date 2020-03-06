#pragma once

struct SlotInfo
{
	unsigned int vertexDataIndex;
	float3 padding;
	// MaterialIndex
	// Texture_Diffuse;
	// TextureNormal:
	// Texture Specular;
};

struct CB_PER_OBJECT
{
	float4x4 worldMatrix;
	float4x4 WVP;
	SlotInfo info;
};
