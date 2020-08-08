#include "BaseCamera.h"

BaseCamera::BaseCamera()
{
	// Create View Matrix
	this->eyeVector = XMVectorSet(0.0, 3.0, -5.0, 1.0f);
	this->atVector = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	this->upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	this->viewMatrix = XMMatrixLookAtLH(this->eyeVector, this->atVector + this->eyeVector, this->upVector);

	this->rightVector = XMVector3Cross(this->atVector, this->upVector);
}

BaseCamera::~BaseCamera()
{

}

void BaseCamera::SetPosition(float x, float y, float z)
{
	this->eyeVector = XMVectorSet(x, y, z, 1.0f);
}


XMFLOAT3 BaseCamera::GetPosition() const
{
	XMFLOAT3 DXfloat3;
	XMStoreFloat3(&DXfloat3, this->eyeVector);

	return DXfloat3;
}

float3 BaseCamera::GetPositionFloat3() const
{
	XMFLOAT3 DXfloat3;
	XMStoreFloat3(&DXfloat3, this->eyeVector);

	float3 temp = {};
	temp.x = DXfloat3.x;
	temp.y = DXfloat3.y;
	temp.z = DXfloat3.z;

	return temp;
}

XMFLOAT3 BaseCamera::GetLookAt() const
{
	XMFLOAT3 DXfloat3;
	XMStoreFloat3(&DXfloat3, this->atVector);

	return DXfloat3;
}

float3 BaseCamera::GetLookAtFloat3() const
{
	XMFLOAT3 DXfloat3;
	XMStoreFloat3(&DXfloat3, this->atVector);

	float3 temp = {};
	temp.x = DXfloat3.x;
	temp.y = DXfloat3.y;
	temp.z = DXfloat3.z;

	return temp;
}
