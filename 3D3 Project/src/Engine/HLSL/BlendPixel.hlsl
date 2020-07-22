#include "../structs.h"

struct VS_OUT
{
	float4 pos      : SV_Position;
	float4 uv       : UV;
	float4 norm     : NORMAL;
	float4 tang     : TANGENT;
	float4 worldPos : WPos;
};

ConstantBuffer<CB_PER_FRAME> cbPerFrame : register(b1);
ConstantBuffer<CB_DirectionalLight> dirLight[] : register(b3);

ConstantBuffer<CB_PER_OBJECT> transform : register(b0);
Texture2D textures[] : register (t0);
SamplerState samLinear : register (s0);

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	float3 camPos = cbPerFrame.camPos;

	float4 lightPos = float4(1.0f, 1.0f, 1.0f, 1.0f); //dirLight[10].position;
	float4 lightColor = float4(1.0f, 1.0f, 1.0f, 1.0f); //dirLight[10].color;
	float4 materialColor = textures[transform.info.Texture_Diffuse].Sample(samLinear, input.uv);

	// Ambient
	float4 ambient = materialColor * float4(0.3f, 0.3f, 0.3f, 1.0f);

	// Diffuse
	float4 lightDir = normalize(lightPos - input.worldPos);
	float alpha = max(dot(input.norm, lightDir), 0.0f);
	float4 diffuse = materialColor * lightColor * alpha;

	// Specular
	float3 vecToCam = normalize(camPos - input.worldPos.xyz);
	float3 reflection = normalize(reflect(normalize(-lightDir.xyz), normalize(input.norm.xyz)));
	float spec = pow(max(dot(reflection, vecToCam), 0.0), 100);
	float3 finalSpecular = materialColor * lightColor * spec;

	// Attenuation
	float distancePixelToLight = length(lightPos - input.worldPos.xyz);
	float attenuation = 1.0f / (1.0f + (0.1 * distancePixelToLight) + (0.01 * pow(distancePixelToLight, 2)));

	// FinalColor
	float blendFactor = 0.2f;
	float4 finalColor = float4(ambient.rgb + attenuation * (diffuse.rgb + finalSpecular.rgb), blendFactor);

	finalColor = saturate(finalColor);
	return finalColor;
}