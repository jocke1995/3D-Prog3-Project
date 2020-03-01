struct VS_OUT
{
	float4 pos : SV_Position;
};

StructuredBuffer<float4> vbPos : register(t0);

VS_OUT VS_main(uint vID : SV_VertexID)
{
	// 0:ar output
	VS_OUT output = (VS_OUT)0;
	output.pos = float4(vbPos[vID].xyz, 1.0);

	return output;
}