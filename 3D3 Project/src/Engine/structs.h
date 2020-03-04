#ifndef STRUCTS_H
#define STRUCTS_H


#ifdef IS_SHADER

	struct CB_PER_OBJECT
	{
		//SlotInfo info;
		float4x4 worldMatrix;
		float4x4 WVP;
	};

	struct SlotInfo
	{
		unsigned int vertexDataIndex;
	};

#else

	struct CB_PER_OBJECT
	{
		//SlotInfo info;
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 WVP;
	};

	struct SlotInfo
	{
		UINT vertexDataIndex = 0;
		// MaterialIndex
		// Texture_Diffuse;
		// TextureNormal:
		// Texture Specular;
	};

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


#endif


#endif