#include "../structs.h"

struct VS_OUT
{
	float4 pos      : SV_Position;
	float4 uv       : UV;
	float4 norm     : NORMAL;
	float4 tang     : TANGENT;
	float4 worldPos : WPos;
};

struct vertex
{
	float4 pos;
	float4 uv;
	float4 norm;
	float4 tang;
};

StructuredBuffer<vertex> meshes[] : register(t0);

ConstantBuffer<CB_PER_OBJECT> transform : register(b0);

VS_OUT VS_main(uint vID : SV_VertexID)
{
	// 0:ar output
	VS_OUT output = (VS_OUT)0;

	vertex mesh = meshes[transform.info.vertexDataIndex][vID];
	float4 vertexPosition = float4(mesh.pos.xyz, 1.0f);

	output.pos = mul(vertexPosition, transform.WVP);
	output.worldPos = mul(vertexPosition, transform.worldMatrix);

	output.uv = float4(mesh.uv);
	output.norm = mul(float4(mesh.norm), transform.worldMatrix);

	output.tang = 0;

	return output;
}