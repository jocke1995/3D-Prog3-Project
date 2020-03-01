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

// this will only call release if an object exists (prevents exceptions calling release on non existant objects)
#define SAFE_RELEASE(p)			\
{								\
	if ((*p))					\
	{							\
		(*p)->Release();		\
		(*p) = NULL;			\
	}							\
}

#define NUM_SWAP_BUFFERS 2

// TODO: Fyll upp senare när vi vet vad vi ska ha för saker.
enum RS
{
	POSITION
};

union float4
{
	struct { float x; float y; float z; float w; };
	struct { float r; float g; float b; float a; };
};

union float3
{
	struct { float x; float y; float z; };
	struct { float r; float g; float b; };
};

union float2
{
	struct { float u; float v; };
	struct { float x; float y; };
};
