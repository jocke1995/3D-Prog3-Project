#include "../structs.h"

struct VS_OUT
{
	float4 pos      : SV_Position;
	float4 worldPos : WPos;
	float4 uv       : UV;
	float4 norm     : NORMAL;
};

struct color
{
	float4 rgba;
};

ConstantBuffer<CB_PER_FRAME> cbPerFrame : register(b1);
ConstantBuffer<color> materialColor0 : register(b2);
ConstantBuffer<CB_DirectionalLight> dirLight[] : register(b3);

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	float3 camPos = cbPerFrame.camPos;

	float4 lightPos = float4(1.0f, 1.0f, 1.0f, 1.0f); //dirLight[10].position;
	float4 lightColor = float4(1.0f, 1.0f, 1.0f, 1.0f); //dirLight[10].color;
	float4 materialColor = materialColor0.rgba;

	// Ambient
	float4 ambient = materialColor * float4(0.2f, 0.2f, 0.2f, 0.2f);
	
	// Diffuse
	float4 lightDir = float4(normalize(lightPos.xyz - input.worldPos.xyz), 1.0f);
	float alpha = max(dot(input.norm.xyz, lightDir.xyz), 0.0f);
	float4 diffuse = materialColor * lightColor * alpha;

	// Specular
	float3 vecToCam = normalize(camPos - input.worldPos.xyz);
	float3 reflection = normalize(reflect(normalize(-lightDir.xyz), normalize(input.norm.xyz)));
	float spec = pow(max(dot(reflection, vecToCam), 0.0), 100);
	float3 finalSpecular = materialColor * lightColor * spec;

	// Attenuation
	float distancePixelToLight = length(lightPos - input.worldPos.xyz);
	float attenuation = 1.0f / (1.0f + (0.1 * distancePixelToLight) + (0.01 * pow(distancePixelToLight, 2)));

	float4 finalColor = float4(ambient.rgb + attenuation * (diffuse.rgb + finalSpecular.rgb), 1.0f);
	finalColor = saturate(finalColor);

	return finalColor;
}