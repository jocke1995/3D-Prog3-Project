#ifndef CAMERA_H
#define CAMERA_H

// TEMP
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

#include "stdafx.h"
#include "Transform.h"

class Camera
{
public:
    Camera(std::wstring name, HINSTANCE hInstance, HWND hwnd);
	~Camera();

    void Update(double dt);

    void SetPosition(float x, float y, float z);

    void SetRotation(XMFLOAT3 axis, float angle);
    void RotateX(float angle);
    void RotateY(float angle);
    void RotateZ(float angle);

    XMFLOAT4X4* GetViewProjMatrix();

private:
    std::wstring name;

    float movementSpeed = 10.0f;
    float moveLeftRight = 0.0f;
    float moveForwardBackward = 0.0f;

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

    // TODO: STEFAN

    /*
    // Initialize the view matrix, ideally this should be based on user
  // interactions The lookat and perspective matrices used for rasterization are
  // defined to transform world-space vertices into a [0,1]x[0,1]x[0,1] camera
  // space
    XMVECTOR Eye = XMVectorSet(1.5f, 1.5f, 1.5f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    matrices[0] = XMMatrixLookAtRH(Eye, At, Up);

    float fovAngleY = 45.0f * XM_PI / 180.0f;
    matrices[1] =
        XMMatrixPerspectiveFovRH(fovAngleY, m_aspectRatio, 0.1f, 1000.0f);

    // Raytracing has to do the contrary of rasterization: rays are defined in
    // camera space, and are transformed into world space. To do this, we need to
    // store the inverse matrices as well.
    XMVECTOR det;
    matrices[2] = XMMatrixInverse(&det, matrices[0]);
    matrices[3] = XMMatrixInverse(&det, matrices[1]);

    // Copy the matrix contents
    uint8_t* pData;
    ThrowIfFailed(m_cameraBuffer->Map(0, nullptr, (void**)&pData));
    memcpy(pData, matrices.data(), m_cameraBufferSize);
    m_cameraBuffer->Unmap(0, nullptr);

    */
};

#endif