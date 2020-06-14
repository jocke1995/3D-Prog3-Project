struct VS_OUT
{
	float4 pos      : SV_Position;
	float4 worldPos : WPos;
	float4 uv       : UV;
	float4 norm     : NORMAL;
};

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	float4 lightPos = float4(3.0f, 5.0f, -5.0f, 1.0f);
	float4 lightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 materialColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// Ambient
	float4 ambient = materialColor * float4(0.3f, 0.3f, 0.3f, 1.0f);

	// Diffuse
	float4 lightDir = normalize(lightPos - input.worldPos);
	float alpha = max(dot(input.norm, lightDir), 0.0f);
	float4 diffuse = materialColor * lightColor * alpha;

	// FinalColor
	float blendFactor = 0.2f;
	float4 finalColor = float4(ambient.rgb + diffuse.rgb, blendFactor);

	finalColor = saturate(finalColor);
	return finalColor;
}