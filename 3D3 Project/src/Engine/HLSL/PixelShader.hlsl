struct VS_OUT
{
	float4 pos : SV_Position;
};

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	//float4 lightPos = float(2.0f, 0.0f, 0.0f, 1.0f);


	return float4(0.5f, 0.2f, 0.5f, 1.0f);
}