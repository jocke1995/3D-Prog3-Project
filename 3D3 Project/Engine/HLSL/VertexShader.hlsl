struct VS_OUT
{
	float4 pos : SV_Position;
};

//StructuredBuffer<float4> vbPos : register(t0);

VS_OUT VS_main(uint vID : SV_VertexID)
{
	// 0:ar output
	VS_OUT output = (VS_OUT)0;
	//float4 position = float4(vbPos[vID].xyz, 1.0);

	if (vID == 0)
		output.pos = float4(-0.5f, -0.5f, 1.0f, 1.0f);
	else if (vID == 1)
		output.pos = float4(0.0f, 0.5f, 1.0f, 1.0f);
	else if (vID == 2)
		output.pos = float4(0.5f, -0.5f, 1.0f, 1.0f);

	return output;
}