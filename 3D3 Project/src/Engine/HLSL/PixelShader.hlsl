#include "LightCalculations.hlsl"

struct VS_OUT
{
	float4 pos      : SV_Position;
	float4 worldPos : WPos;
	float4 uv       : UV;
	float3x3 tbn	: TBN;
};

ConstantBuffer<CB_PER_OBJECT_STRUCT> perObject : register(b0);	// Texture-ids is stored here
ConstantBuffer<CB_PER_FRAME_STRUCT> cbPerFrame : register(b1);
ConstantBuffer<CB_PER_SCENE_STRUCT> cbPerScene : register(b2);

ConstantBuffer<DirectionalLight> dirLight[]    : register(b3, space0);
ConstantBuffer<PointLight> pointLight[]		   : register(b3, space1);
ConstantBuffer<SpotLight> spotLight[]		   : register(b3, space2);

Texture2D textures[] : register (t0);
SamplerState samLinear : register (s0);

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	float3 camPos = cbPerFrame.camPos;
	float3 finalColor = float3(0.0f, 0.0f, 0.0f);

	// Sample from textures
	float4 ambientMap  = textures[perObject.info.textureAmbient ].Sample(samLinear, input.uv);
	float4 diffuseMap  = textures[perObject.info.textureDiffuse ].Sample(samLinear, input.uv);
	float4 specularMap = textures[perObject.info.textureSpecular].Sample(samLinear, input.uv);
	float4 emissiveMap = textures[perObject.info.textureEmissive].Sample(samLinear, input.uv);
	float4 normalMap   = textures[perObject.info.textureNormal  ].Sample(samLinear, input.uv);

	
	normalMap = (2.0f * normalMap) - 1.0f;
	float4 normal = float4(normalize(mul(normalMap.xyz, input.tbn)), 1.0f);

	// DirectionalLight contributions
	for (unsigned int i = 0; i < cbPerScene.Num_Dir_Lights; i++)
	{
		finalColor += CalcDirLight(dirLight[cbPerScene.dirLightIndices[i].x], camPos, input.worldPos.xyz,
			ambientMap.rgb,
			diffuseMap.rgb,
			specularMap.rgb,
			normal.rgb);
	}

	// PointLight contributions
	for (unsigned int i = 0; i < cbPerScene.Num_Point_Lights; i++)
	{
		finalColor += CalcPointLight(pointLight[cbPerScene.pointLightIndices[i].x], camPos, input.worldPos.xyz,
			ambientMap.rgb,
			diffuseMap.rgb,
			specularMap.rgb,
			normal.rgb);
	}

	// SpotLight  contributions
	for (unsigned int i = 0; i < cbPerScene.Num_Spot_Lights; i++)
	{
		finalColor += CalcSpotLight(spotLight[cbPerScene.spotLightIndices[i].x], camPos, input.worldPos.xyz,
			ambientMap.rgb,
			diffuseMap.rgb,
			specularMap.rgb,
			normal.rgb);
	}

	finalColor += emissiveMap.rgb;
	finalColor = saturate(finalColor);
	return float4(finalColor.rgb, 1.0f);
}