#ifndef STRUCTS_H
#define STRUCTS_H

// Indicies of where the descriptors are stored in the descriptorHeap
struct SlotInfo
{
	unsigned int vertexDataIndex;
	// TextureIndices
	unsigned int textureAmbient;
	unsigned int textureDiffuse;
	unsigned int textureSpecular;
	unsigned int textureNormal;
	unsigned int textureEmissive;
	float2 padding;
};

struct CB_PER_OBJECT
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

struct CB_DirectionalLight
{
	float4 position;
	float4 color;
};

struct Lights
{
	CB_DirectionalLight dirLights[256];
	float Num_Dir_Lights;
};

#endif