struct color
{
	float4 rgba;
};

RWStructuredBuffer<color> materialColor0 : register(u0);

[numthreads(3, 1, 1)] // ? Man skriver ju detta i dispatch
void CS_main(uint3 DTid : SV_DispatchThreadID)
{
	// threadID = threadIDx.x + blockIdx.x * blockDim.x
	// threadIDx.x + threadIDx.y * threadIDx.z ??
	int threadId = DTid.x + DTid.y * DTid.z;

	if (threadId <= 3)
	{
		materialColor0[0].rgba[threadId] *= 10.0f;
		materialColor0[0].rgba[threadId] = abs(sin(materialColor0[0].rgba[threadId]));
	}
	
}