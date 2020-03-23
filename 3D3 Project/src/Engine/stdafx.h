#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include "d3dx12.h"

// Need to declare before including structs.h
typedef DirectX::XMFLOAT4X4 float4x4;
typedef union
{
	struct { float x; float y; float z; float w; };
	struct { float r; float g; float b; float a; };
} float4;

typedef union
{
	struct { float x; float y; float z; };
	struct { float r; float g; float b; };
} float3;

typedef union
{
	struct { float x; float y; };
	struct { float u; float v; };
} float2;

#include "structs.h"

// this will only call release if an object exists (prevents exceptions calling release on non existant objects)
#define SAFE_RELEASE(p)			\
{								\
	if ((*p))					\
	{							\
		(*p)->Release();		\
		(*p) = NULL;			\
	}							\
}

enum RENDER_TASK_TYPE
{
	TEST,
	BLEND,
	NR_OF_RENDERTASKS
};

enum COPY_TASK_TYPE
{
	COPY_COLOR,
	NR_OF_COPYTASKS
};

enum COMMAND_INTERFACE_TYPE
{
	DIRECT_TYPE,
	COPY_TYPE,
	COMPUTE_TYPE
};

#define NUM_SWAP_BUFFERS 2


