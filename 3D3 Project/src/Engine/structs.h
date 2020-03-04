#ifndef STRUCTS_H
#define STRUCTS_H


#ifdef IS_SHADER

	struct CB_PER_OBJECT
	{
		float4x4 worldMatrix;
		//float4x4 WVP;
	};

#else

	struct CB_PER_OBJECT
	{
		DirectX::XMFLOAT4X4 worldMatrix;
		//DirectX::XMFLOAT4X4 WVP;
	};

	struct SlotInfo
	{
		UINT vertexDataIndex = 0;
		// MaterialIndex
		// Texture_Diffuse;
		// TextureNormal:
		// Texture Specular;
	};

#endif


#endif