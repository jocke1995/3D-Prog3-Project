struct VS_OUT
{
	float4 pos      : SV_Position;
	float4 worldPos : WPos;
	float4 uv       : UV;
	float4 norm     : NORMAL;
};

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	float4 lightPos = float4(0.5f, 2.0f, 0.0f, 1.0f);
	float4 lightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 materialColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// Ambient
	float4 ambient = materialColor * float4(0.2f, 0.2f, 0.2f, 0.2f);

	// Diffuse
	float4 lightDir = float4(normalize(lightPos.xyz - input.worldPos.xyz), 1.0f);
	float alpha = max(dot(input.norm.xyz, lightDir.xyz), 0);
	float4 diffuse = materialColor * lightColor * alpha;

	// FinalColor
	float4 finalColor = float4(ambient.rgb + diffuse.rgb, 1.0f);

	return finalColor;
}