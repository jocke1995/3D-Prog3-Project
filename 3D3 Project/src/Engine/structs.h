#pragma once

#ifdef IS_SHADER

	struct SlotInfo
	{
		uint vertexDataIndex;
	};

	struct CB_PER_OBJECT
	{
		float4x4 worldMatrix;
		float4x4 WVP;
		SlotInfo info;
	};

#else

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

	struct alignas(16) SlotInfo
	{
		unsigned int vertexDataIndex = 0;
		// MaterialIndex
		// Texture_Diffuse;
		// TextureNormal:
		// Texture Specular;
	};

	struct CB_PER_OBJECT
	{
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 WVP;
		SlotInfo info;
	};

	


#endif
