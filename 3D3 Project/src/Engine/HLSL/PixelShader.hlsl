#include "../structs.h"

struct VS_OUT
{
	float4 pos      : SV_Position;
	float4 worldPos : WPos;
	float4 uv       : UV;
	float3x3 tbn	: TBN;
};

ConstantBuffer<CB_PER_OBJECT> transform : register(b0);	// Texture-ids is stored here
ConstantBuffer<CB_PER_FRAME> cbPerFrame : register(b1);
ConstantBuffer<CB_DirectionalLight> dirLight[] : register(b3);

Texture2D textures[] : register (t0);
SamplerState samLinear : register (s0);

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	float3 camPos = cbPerFrame.camPos;

	float4 lightPos = float4(3.0f, 2.0f, 0.0f, 1.0f); //dirLight[10].position;
	float4 lightColor = float4(0.7f, 0.2f, 0.2f, 1.0f); //dirLight[10].color;
	float4 ambientMap = textures[transform.info.textureAmbient].Sample(samLinear, input.uv);
	float4 diffuseMap = textures[transform.info.textureDiffuse].Sample(samLinear, input.uv);
	float4 specularMap = textures[transform.info.textureSpecular].Sample(samLinear, input.uv);
	float4 emissiveMap = textures[transform.info.textureEmissive].Sample(samLinear, input.uv);

	// Sample from normalMap
	float4 normalMap = textures[transform.info.textureNormal].Sample(samLinear, input.uv);
	normalMap = (2.0f * normalMap) - 1.0f;
	float3 normal = normalize(mul(normalMap.xyz, input.tbn));

	// Ambient
	float4 ambient = ambientMap;
	
	// Diffuse
	float4 lightDir = float4(normalize(lightPos.xyz - input.worldPos.xyz), 1.0f);
	float alpha = max(dot(normal.xyz, lightDir.xyz), 0.0f);
	float4 diffuse = diffuseMap * lightColor * alpha;

	// Specular
	float3 vecToCam = normalize(camPos - input.worldPos.xyz);
	float3 reflection = normalize(reflect(normalize(-lightDir.xyz), normalize(normal.xyz)));
	float spec = pow(max(dot(reflection, vecToCam), 0.0), 10);
	float3 finalSpecular = specularMap.rgb * lightColor * spec;

	// Attenuation
	float distancePixelToLight = length(lightPos - input.worldPos.xyz);
	float attenuation = 1.0f / (1.0f + (0.1 * distancePixelToLight) + (0.01 * pow(distancePixelToLight, 2)));

	float4 finalColor = float4(ambient.rgb + attenuation * (diffuse.rgb + finalSpecular.rgb) + emissiveMap.rgb, 1.0f) * 2;
	finalColor = saturate(finalColor);

	return finalColor;
}