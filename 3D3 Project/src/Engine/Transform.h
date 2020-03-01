#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "stdafx.h"

#include "ConstantBuffer.h"

using namespace DirectX;

class Transform
{
public:
	Transform(ConstantBuffer* constantBufferPosition);
	~Transform();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 pos);
	
	void SetRotation(XMFLOAT3 axis, float angle);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);


	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3 scale);


	ConstantBuffer* GetConstantBuffer();
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress();

private:
	XMFLOAT3 position;
	XMFLOAT4X4 rotationMat;
	XMFLOAT3 scale;

	XMFLOAT4X4 worldMat;

	ConstantBuffer* constantBuffer;
	void* constantBufferOffset;

	void UpdateWorldMatrix();

};

#endif