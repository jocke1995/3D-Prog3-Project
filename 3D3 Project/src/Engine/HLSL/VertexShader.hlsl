#include "../structs.h"

struct VS_OUT
{
	float4 pos : SV_Position;
};

struct vertex
{
	float4 pos;
	//float4 uv;
	//float4 norm;
};

StructuredBuffer<vertex> meshes[] : register(t0);

ConstantBuffer<CB_PER_OBJECT> transform : register(b0);

VS_OUT VS_main(uint vID : SV_VertexID)
{
	// 0:ar output
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(meshes[transform.info.vertexDataIndex][vID].pos.xyz, 1.0);
	// output.pos = transform.WVP[0];
	output.pos = mul(output.pos, transform.WVP);

	return output;
}