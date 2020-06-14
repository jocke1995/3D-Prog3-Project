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

ConstantBuffer<color> materialColor0 : register(b1);

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	float4 lightPos = float4(3.0f, 5.0f, -5.0f, 1.0f);
	float4 lightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 materialColor = materialColor0.rgba;

	// Ambient
	float4 ambient = materialColor * float4(0.2f, 0.2f, 0.2f, 0.2f);
	
	// Diffuse
	float4 lightDir = float4(normalize(lightPos.xyz - input.worldPos.xyz), 1.0f);
	float alpha = max(dot(input.norm.xyz, lightDir.xyz), 0.0f);
	float4 diffuse = materialColor * lightColor * alpha;

	float4 finalColor = float4(diffuse.rgb + ambient.rgb, 1.0f);
	finalColor = saturate(finalColor);

	return finalColor;
}