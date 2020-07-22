#ifndef STRUCTS_H
#define STRUCTS_H

// Indicies of where the data is stored in GPU-memory
struct SlotInfo
{
	unsigned int vertexDataIndex;
	float3 padding1;
	// MaterialIndex
	unsigned int textureAmbient;
	float3 padding2;
	unsigned int textureDiffuse;
	float3 padding3;
	unsigned int textureSpecular;
	float3 padding4;
	unsigned int textureNormal;
	float3 padding5;
	unsigned int textureEmissive;
	float3 padding6;
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