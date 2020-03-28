struct color
{
	float4 rgba;
};

RWStructuredBuffer<color> materialColor0 : register(u0);

[numthreads(3, 1, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID)
{
	// threadID = threadIDx.x + blockIdx.x * blockDim.x
	int threadId = DTid.x + DTid.y * DTid.z;

	materialColor0[0].rgba[threadId] *= 2;
}