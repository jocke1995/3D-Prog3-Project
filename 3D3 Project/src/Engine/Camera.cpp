#include "Camera.h"

Camera::Camera(std::wstring name, HINSTANCE hInstance, HWND hwnd)
{
	this->name = name;

	// Create Projection Matrix
	float fovAngleY = 45.0f * XM_PI / 180.0f;
	float aspectRatio = 16.0f/9.0f;
	float zNear = 0.1f;
	float zFar = 1000.0f;
	XMMATRIX tempProj = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, zNear, zFar);
	XMStoreFloat4x4(&this->projMat, tempProj);

	// Create View Matrix
	this->eyeVector = XMVectorSet(0.0, 3.0, -5.0, 1.0f);
	this->atVector = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	this->upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX tempView = XMMatrixLookAtLH(this->eyeVector, this->atVector + this->eyeVector, this->upVector);

	XMStoreFloat4x4(&this->viewMat, tempView);

	this->rightVector = XMVector3Cross(this->atVector, this->upVector);

	this->camRotationMatrix = XMMatrixIdentity();
	this->viewMatMatrix = XMMatrixIdentity();
	this->InitDirectInput(hInstance, hwnd);
}

Camera::~Camera()
{
	this->keyboard->Unacquire();
	this->mouse->Unacquire();

	this->DirectInput->Release();
}

void Camera::Update(double dt)
{
	this->DetectInput(dt);
	this->UpdateViewProjMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->eyeVector = XMVectorSet(x, y, z, 1.0f);
}

// void Camera::SetRotation(XMFLOAT3 axis, float angle)
// {
// }

// void Camera::RotateX(float angle)
// {
// }
// 
// void Camera::RotateY(float angle)
// {
// }
// 
// void Camera::RotateZ(float angle)
// {
// }

const XMFLOAT4X4* const Camera::GetViewProjMatrix() const
{
	return &this->viewProjMat;
}

XMFLOAT3 Camera::GetPosition() const
{
	XMFLOAT3 DXfloat3;
	XMStoreFloat3(&DXfloat3, this->eyeVector);

	return DXfloat3;
}

float3 Camera::GetPositionFloat3() const
{
	XMFLOAT3 DXfloat3;
	XMStoreFloat3(&DXfloat3, this->eyeVector);

	float3 temp = {};
	temp.x = DXfloat3.x;
	temp.y = DXfloat3.y;
	temp.z = DXfloat3.z;

	return temp;
}

XMFLOAT3 Camera::GetLookAt() const
{
	XMFLOAT3 DXfloat3;
	XMStoreFloat3(&DXfloat3, this->atVector);

	return DXfloat3;
}

void Camera::UpdateCamera()
{
	// Update the lookAt Vector depending on the mouse pitch/yaw.... WE DONT HAVE ROLL (hence '0' as the last parameter)
	this->camRotationMatrix = XMMatrixRotationRollPitchYaw(this->camPitch, this->camYaw, 0);
	XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	this->atVector = XMVector3TransformCoord(defaultForward, this->camRotationMatrix);
	this->atVector = XMVector3Normalize(this->atVector);

	// Update cameras forward,up and right vectors
	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

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

	this->viewMatMatrix = XMMatrixLookAtLH(this->eyeVector, this->eyeVector + this->atVector, this->upVector);
}

void Camera::InitDirectInput(HINSTANCE hInstance, HWND hwnd)
{
	// Init DirectInput
	HRESULT hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL);
	if (FAILED(hr))
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to Create Direct Input\n");

	// Create Device for Keyboard
	hr = DirectInput->CreateDevice(GUID_SysKeyboard,
		&this->keyboard,
		NULL);
	if (FAILED(hr))
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to Create device for keyboard\n");

	// Create Device for Mouse
	hr = DirectInput->CreateDevice(GUID_SysMouse,
		&this->mouse,
		NULL);
	if (FAILED(hr))
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to Create device for mouse\n");

	// Init Keyboard
	hr = this->keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to SetDataFormat for keyboard\n");
	hr = this->keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to SetCooperativeLevel for keyboard\n");

	// Init Mouse
	hr = this->mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to SetDataFormat for mouse\n");
	hr = this->mouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	if (FAILED(hr))
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to SetCooperativeLevel for mouse\n");
}

void Camera::DetectInput(double dt)
{
	DIMOUSESTATE mouseCurrState;

	// Array of possible keys to be pressed
	unsigned char keyboardState[256];

	this->keyboard->Acquire();
	this->mouse->Acquire();	// Takes control over the mouse

	this->mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
	this->keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (keyboardState[DIK_W] & 0x80)
		this->moveForwardBackward += this->movementSpeed * dt;

	if (keyboardState[DIK_S] & 0x80)
		this->moveForwardBackward -= this->movementSpeed * dt;

	if (keyboardState[DIK_A] & 0x80)
		this->moveLeftRight += this->movementSpeed * dt;

	if (keyboardState[DIK_D] & 0x80)
		this->moveLeftRight -= this->movementSpeed * dt;

	if (keyboardState[DIK_R] & 0x80)
		this->moveUpDown += this->movementSpeed * dt;

	if (keyboardState[DIK_F] & 0x80)
		this->moveUpDown -= this->movementSpeed * dt;

	if ((mouseCurrState.lX != this->mouseLastState.lX) || (mouseCurrState.lY != this->mouseLastState.lY))
	{
		this->camYaw += this->mouseLastState.lX * 0.001f;

		this->camPitch += mouseCurrState.lY * 0.001f;

		this->mouseLastState = mouseCurrState;
	}
	this->UpdateCamera();
}

void Camera::UpdateViewProjMatrix()
{
	XMMATRIX tempProj = XMLoadFloat4x4(&this->projMat);
	XMMATRIX tempVP = this->viewMatMatrix * tempProj;

	XMStoreFloat4x4(&this->viewProjMat, (tempVP));
}
