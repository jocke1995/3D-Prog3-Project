#ifndef CAMERA_H
#define CAMERA_H

// TEMP
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

#include "Transform.h"

class Camera
{
public:
    Camera(std::wstring name, HINSTANCE hInstance, HWND hwnd);
	~Camera();

    void Update(double dt);

    void SetPosition(float x, float y, float z);

    // void SetRotation(XMFLOAT3 axis, float angle);
    // void RotateX(float angle);
    // void RotateY(float angle);
    // void RotateZ(float angle);

    const XMFLOAT4X4* const GetViewProjMatrix() const;
    XMFLOAT3 GetPosition() const;
    XMFLOAT3 GetLookAt() const;

private:
    std::wstring name;

    float movementSpeed = 10.0f;
    float moveLeftRight = 0.0f;
    float moveForwardBackward = 0.0f;
    float moveUpDown = 0.0f;

    XMVECTOR rightVector;
    XMVECTOR eyeVector;
    XMVECTOR atVector;
    XMVECTOR upVector;

    // INPUT
    IDirectInputDevice8* keyboard;
    IDirectInputDevice8* mouse;
    DIMOUSESTATE mouseLastState;
    LPDIRECTINPUT8 DirectInput;

    XMFLOAT4X4 viewMat;
    XMFLOAT4X4 projMat;
    XMFLOAT4X4 viewProjMat;

    float camYaw = 0.0f;
    float camPitch = 0.0f;
    XMMATRIX camRotationMatrix;
    XMMATRIX viewMatMatrix;
    void UpdateCamera();

    void InitDirectInput(HINSTANCE hInstance, HWND hwnd);
    void DetectInput(double dt);
    void UpdateViewProjMatrix();
};

#endif