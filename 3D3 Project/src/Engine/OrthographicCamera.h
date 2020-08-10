#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "BaseCamera.h"

class OrthographicCamera : public BaseCamera
{
public:
    OrthographicCamera(
        XMVECTOR position = { 0.0, 3.0, -5.0, 1.0f },
        XMVECTOR lookAt = { 0.0f, 0.0f, 1.0f, 1.0f },
        float left = -15.0f,
        float right = 15.0f,
        float bot = -15.0f,
        float top = 15.0f,
        float nearZ = 0.1f,
        float farZ = 1000.0f);
    virtual ~OrthographicCamera();

    XMMATRIX* GetViewProjection();

private:
    float moveLeftRight = 0.0f;
    float moveForwardBackward = 0.0f;
    float moveUpDown = 0.0f;
    float camYaw = 0.0f;
    float camPitch = 0.0f;

    XMMATRIX projMatrix;
    XMMATRIX viewProjMatrix;

    void UpdateSpecific(double dt);
};

#endif
