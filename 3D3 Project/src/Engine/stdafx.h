#pragma once
#define NOMINMAX	// Assimp is using a function called "min"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include "d3dx12.h"

// Windows stuff

#include <Windows.h>

// Time
#include <chrono>
#include <ctime>

// Stuff
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

// Threads
#include <mutex>
#include <process.h>	// _beginThreadex

// Data Structures
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>

// For wstring convertion
#include <locale>
#include <codecvt>

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

enum DrawOptions
{
	ForwardRendering = 0x01,
	Blend = 0x02,
	// Shadow = 0x04,
	// Bloom = 0x08,
	// etc..
};

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


#define NUM_SWAP_BUFFERS 2


