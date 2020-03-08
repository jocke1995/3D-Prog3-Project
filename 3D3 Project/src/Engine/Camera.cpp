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
	this->eyeVector = XMVectorSet(0.0, 0.0, 0.0, 1.0f);
	this->atVector = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	this->upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX tempView = XMMatrixLookAtLH(this->eyeVector, this->atVector + this->eyeVector, this->upVector);

	XMStoreFloat4x4(&this->viewMat, tempView);

	this->rotateAroundVector = XMVector3Cross(this->atVector, this->upVector);

	this->InitDirectInput(hInstance, hwnd);
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
	this->DetectInput(dt);
	this->UpdateViewProjMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->eyeVector = XMVectorSet(x, y, z, 1.0f);
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

XMFLOAT4X4* Camera::GetViewProjMatrix()
{
	return &this->viewProjMat;
}

void Camera::MoveForward(float dt)
{
	for (int i = 0; i < 3; i++)
	{
		this->eyeVector.m128_f32[i] += this->movementSpeed * this->atVector.m128_f32[i] * dt;
	}
}

void Camera::MoveBackward(float dt)
{
	for (int i = 0; i < 3; i++)
	{
		this->eyeVector.m128_f32[i] -= this->movementSpeed * this->atVector.m128_f32[i] * dt;
	}
}

void Camera::MoveRight(float dt)
{
	for (int i = 0; i < 3; i++)
	{
		this->eyeVector.m128_f32[i] -= this->movementSpeed * this->rotateAroundVector.m128_f32[i] * dt;
	}
}

void Camera::MoveLeft(float dt)
{
	for (int i = 0; i < 3; i++)
	{
		this->eyeVector.m128_f32[i] += this->movementSpeed * this->rotateAroundVector.m128_f32[i] * dt;
	}
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
		OutputDebugStringW(L"ERROR: Failed to Create DirectInput\n");

	// Create Device for Keyboard
	hr = DirectInput->CreateDevice(GUID_SysKeyboard,
		&this->keyboard,
		NULL);
	if (FAILED(hr))
		OutputDebugStringW(L"ERROR: Failed to Create Device for keyboard\n");

	// Create Device for Mouse
	hr = DirectInput->CreateDevice(GUID_SysMouse,
		&this->mouse,
		NULL);
	if (FAILED(hr))
		OutputDebugStringW(L"ERROR: Failed to Create Device for mouse\n");

	// Init Keyboard
	hr = this->keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		OutputDebugStringW(L"ERROR: Failed to SetDataFormat for keyboard\n");
	hr = this->keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		OutputDebugStringW(L"ERROR: Failed to SetCooperativeLevel for keyboard\n");

	// Init Mouse
	hr = this->mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		OutputDebugStringW(L"ERROR: Failed to SetDataFormat for mouse\n");
	hr = this->mouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	if (FAILED(hr))
		OutputDebugStringW(L"ERROR: Failed to SetCooperativeLevel for mouse\n");
}

void Camera::DetectInput(float dt)
{
	DIMOUSESTATE mouseCurrState;

	// Array of possible keys to be pressed
	unsigned char keyboardState[256];

	this->keyboard->Acquire();
	this->mouse->Acquire();	// Takes control over the mouse

	this->mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
	this->keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if(keyboardState[DIK_W] & 0x80)
		this->MoveForward(dt);

	if (keyboardState[DIK_S] & 0x80)
		this->MoveBackward(dt);

	if (keyboardState[DIK_A] & 0x80)
		this->MoveLeft(dt);

	if (keyboardState[DIK_D] & 0x80)
		this->MoveRight(dt);
}

void Camera::UpdateViewProjMatrix()
{
	// Update viewMatrix position
	XMMATRIX newView = XMMatrixLookAtLH(this->eyeVector, this->atVector + this->eyeVector, this->upVector);
	XMStoreFloat4x4(&this->viewMat, newView);

	//XMMATRIX tempView = XMLoadFloat4x4(&this->viewMat);
	XMMATRIX tempProj = XMLoadFloat4x4(&this->projMat);

	XMMATRIX tempVP = newView * tempProj;

	XMStoreFloat4x4(&this->viewProjMat, (tempVP));
}
