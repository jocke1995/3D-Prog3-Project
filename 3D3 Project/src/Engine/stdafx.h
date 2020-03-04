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


