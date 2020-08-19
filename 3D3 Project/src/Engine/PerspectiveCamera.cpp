#include "PerspectiveCamera.h"

// TEMPORARY CONSTRUCTOR
PerspectiveCamera::PerspectiveCamera(HINSTANCE hInstance, HWND hwnd)
	:BaseCamera()
{
	this->Init();
	this->UpdateSpecific(0);

	this->tempHasInputObject = true;
	this->tempInputClass = new TempInputClass();
	this->tempInputClass->InitDirectInput(hInstance, hwnd);
}

PerspectiveCamera::PerspectiveCamera(XMVECTOR position, XMVECTOR lookAt)
	:BaseCamera(position, lookAt)
{
	this->Init();
	this->UpdateSpecific(0);

	this->tempHasInputObject = false;
}

PerspectiveCamera::~PerspectiveCamera()
{
	if (tempHasInputObject == true)
	{
		delete this->tempInputClass;
	}
}

void PerspectiveCamera::UpdateSpecific(double dt)
{
	if (tempHasInputObject == true)
	{
		this->tempInputClass->DetectInput(dt,
			&this->moveForwardBackward,
			&this->moveLeftRight,
			&this->moveUpDown,
			&this->camYaw,
			&this->camPitch);

		this->UpdateCameraMovement();
	}
	

	this->viewProjMatrix = this->viewMatrix * this->projMatrix;
	this->viewProjTranposedMatrix = DirectX::XMMatrixTranspose(this->viewProjMatrix);
}

XMMATRIX* PerspectiveCamera::GetViewProjection()
{
	return &this->viewProjMatrix;
}

XMMATRIX* PerspectiveCamera::GetViewProjectionTranposed()
{
	return &this->viewProjTranposedMatrix;
}

void PerspectiveCamera::Init()
{
	// Create Projection Matrix
	float fovAngleY = 45.0f * XM_PI / 180.0f;
	float aspectRatio = 16.0f / 9.0f;
	float zNear = 0.1f;
	float zFar = 1000.0f;
	this->projMatrix = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, zNear, zFar);
}

void PerspectiveCamera::UpdateCameraMovement()
{
	// Update the lookAt Vector depending on the mouse pitch/yaw.... WE DONT HAVE ROLL (hence '0' as the last parameter)
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->camPitch, this->camYaw, 0);
	XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	this->atVector = XMVector3TransformCoord(defaultForward, camRotationMatrix);
	this->atVector = XMVector3Normalize(this->atVector);

	// Update cameras forward,up and right vectors
	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(this->camYaw);

	XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	
	this->upVector = XMVector3TransformCoord(this->upVector, RotateYTempMatrix);
	this->atVector = XMVector3TransformCoord(this->atVector, RotateYTempMatrix);

	this->rightVector = XMVector3Cross(this->atVector, this->upVector);

	this->eyeVector += this->rightVector * this->moveLeftRight;
	this->eyeVector += this->atVector * this->moveForwardBackward;
	this->eyeVector += this->upVector * this->moveUpDown;

	this->moveForwardBackward = 0.0f;
	this->moveLeftRight = 0.0f;
	this->moveUpDown = 0.0f;

	this->viewMatrix = XMMatrixLookAtLH(this->eyeVector, this->eyeVector + this->atVector, this->upVector);
}
