#include "Transform.h"

Transform::Transform(ConstantBuffer* constantBuffer)
{
	this->constantBuffer = constantBuffer;
	// Get a constantbufferlocation
	this->constantBufferOffset = constantBuffer->GetValidLocation();

	this->position = XMMatrixIdentity();
	this->rotation = XMMatrixIdentity();
	this->scale = XMMatrixIdentity();
	this->worldMatrix = XMMatrixIdentity();
}

Transform::~Transform()
{
}

void Transform::SetPosition(float x, float y, float z)
{
}

void Transform::SetPosition(XMFLOAT3 pos)
{
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
}

void Transform::SetScale(XMFLOAT3 scale)
{
}

ConstantBuffer* Transform::GetConstantBuffer()
{
	// TODO: insert return statement here

	return this->constantBuffer;
}

void Transform::UpdateWorldMatrix()
{
	this->worldMatrix = this->position * this->rotation * this->scale;

	XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, this->worldMatrix);

	this->constantBuffer->SetData(this->constantBufferOffset, &data, CONSTANT_BUFFER_TYPE::CB_PER_OBJECT);
}
