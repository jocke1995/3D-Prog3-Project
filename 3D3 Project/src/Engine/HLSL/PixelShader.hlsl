struct VS_OUT
{
	float4 pos : SV_Position;
};

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	return float4(0.5f, 0.2f, 0.5f, 1.0f);
}