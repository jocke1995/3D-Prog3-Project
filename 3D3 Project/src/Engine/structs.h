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

struct CB_PER_OBJECT_STRUCT
{
	float4x4 worldMatrix;
	float4x4 WVP;
	SlotInfo info;
};

struct CB_PER_FRAME_STRUCT
{
	float3 camPos;
	// deltaTime ..
	// etc ..
};

struct CB_PER_SCENE_STRUCT
{
	float4 dirLightIndices[10];
	unsigned int Num_Dir_Lights;
	float3 padding1;
	float4 pointLightIndices[10];
	unsigned int Num_Point_Lights;
	float3 padding2;
	float4 spotLightIndices[10];
	unsigned int Num_Spot_Lights;
};

struct BaseLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
};

struct DirectionalLight
{
	float4 direction;
	BaseLight baseLight;
};

struct PointLight
{
	float4 position;
	float4 attenuation;	// 4byte-constant, 4byte-linear, 4byte-quadratic, 4byte-padding
	BaseLight baseLight;
};

struct SpotLight
{
	float4 position_cutOff;			// position  .x.y.z & cutOff in .w (cutOff = radius)
	float4 direction_outerCutoff;	// direction .x.y.z & outerCutOff in .w

	float4 attenuation;	// 4byte-constant, 4byte-linear, 4byte-quadratic, 4byte-padding
	BaseLight baseLight;
};
#endif