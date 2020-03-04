#include "Transform.h"

Transform::Transform()
{
	this->position = XMFLOAT3(0.0, 0.0, 0.0);
	XMStoreFloat4x4(&this->rotationMat, XMMatrixIdentity());
	this->scale = XMFLOAT3(1.0, 1.0, 1.0);

	this->UpdateWorldMatrix();
}

Transform::~Transform()
{
}

void Transform::SetPosition(float x, float y, float z)
{
	this->position = XMFLOAT3(x, y, z);
}

void Transform::SetPosition(XMFLOAT3 pos)
{
	this->position = pos;
}

void Transform::SetRotation(XMFLOAT3 axis, float angle)
{
}

void Transform::RotateX(float angle)
{
}

void Transform::RotateY(float angle)
{
}

void Transform::RotateZ(float angle)
{
}

void Transform::SetScale(float x, float y, float z)
{
	this->scale = XMFLOAT3(x, y, z);
}

void Transform::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
}

XMFLOAT4X4* Transform::GetWorldMatrix()
{
	return &this->worldMat;
}

void Transform::UpdateWorldMatrix()
{
	XMMATRIX tmpWorldMat;

	XMMATRIX posMat = XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
	XMMATRIX sclMat = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
	XMMATRIX rotMat = XMLoadFloat4x4(&this->rotationMat);

	tmpWorldMat = rotMat * sclMat * posMat;

	XMStoreFloat4x4(&this->worldMat, tmpWorldMat);
}
