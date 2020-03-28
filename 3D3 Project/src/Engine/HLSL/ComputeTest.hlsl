struct color
{
	float4 rgba;
};

ConstantBuffer<color> materialColor0 : register(b1);

[numthreads(3, 1, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID)
{
	// threadID = threadIDx.x + blockIdx.x * blockDim.x
	int threadId = DTid.x + DTid.y * DTid.z;

	//materialColor0 *= 2;
}