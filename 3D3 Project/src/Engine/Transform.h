#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "stdafx.h"

using namespace DirectX;

class Transform
{
public:
	Transform();
	~Transform();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 pos);
	
	void SetRotation(XMFLOAT3 axis, float angle);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);

	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3 scale);

	void UpdateWorldMatrix();

	XMFLOAT4X4* GetWorldMatrix();
	XMFLOAT3 GetPosition();

private:
	XMFLOAT3 position;
	XMFLOAT4X4 rotationMat;
	XMFLOAT3 scale;

	XMFLOAT4X4 worldMat;

	XMMATRIX rotXMat;
	XMMATRIX rotYMat;
	XMMATRIX rotZMat;

};

#endif