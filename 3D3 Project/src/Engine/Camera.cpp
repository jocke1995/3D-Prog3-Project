#include "Camera.h"

Camera::Camera(std::wstring name, ConstantBuffer* cb)
{
	this->name = name;
	this->constantBuffer = cb;


	// Create Projection Matrix
	float fovAngleY = 45.0f * XM_PI / 180.0f;
	float aspectRatio = 16.0f/9.0f;
	float zNear = 0.1f;
	float zFar = 1000.0f;
	XMMATRIX tempProj = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, zNear, zFar);
	XMStoreFloat4x4(&this->projMat, tempProj);


	// Create View Matrix
	XMVECTOR Eye = XMVectorSet(1.5f, 1.5f, 1.5f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX tempView = XMMatrixLookAtRH(Eye, At, Up);

	XMStoreFloat4x4(&this->viewMat, tempView);

	this->UpdateViewProjMatrix();
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	this->position = XMFLOAT3(x, y, z);
}

void Camera::SetPosition(XMFLOAT3 pos)
{
	this->position = pos;
}

void Camera::SetRotation(XMFLOAT3 axis, float angle)
{
}

void Camera::RotateX(float angle)
{
}

void Camera::RotateY(float angle)
{
}

void Camera::RotateZ(float angle)
{
}

D3D12_GPU_VIRTUAL_ADDRESS Camera::GetGPUAddress()
{
	ID3D12Resource1* resource = this->constantBuffer->GetResource();
	D3D12_GPU_VIRTUAL_ADDRESS address = resource->GetGPUVirtualAddress();
	//address += (char)this->constantBufferOffset;
	return address;
}

void Camera::UpdateViewProjMatrix()
{
	XMMATRIX tempView = XMLoadFloat4x4(&this->viewMat);
	XMMATRIX tempProj = XMLoadFloat4x4(&this->projMat);

	XMMATRIX tempVP = tempView * tempProj;

	XMStoreFloat4x4(&this->vp, tempVP);
}
