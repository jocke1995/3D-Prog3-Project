#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "BaseCamera.h"

class OrthographicCamera : public BaseCamera
{
public:
    OrthographicCamera(
        XMVECTOR position,
        XMVECTOR lookAt,
        float left = -40.0f,
        float right = 40.0f,
        float bot = -40.0f,
        float top = 40.0f,
        float nearZ = 0.01f,
        float farZ = 1000.0f);
    virtual ~OrthographicCamera();

    XMMATRIX* GetViewProjection();
    XMMATRIX* GetViewProjectionTranposed();

private:
    float left  = 0;
    float right = 0;
    float bot   = 0;
    float top   = 0;
    float nearZ = 0;
    float farZ  = 0;

    XMMATRIX projMatrix;

    XMMATRIX viewProjMatrix;
    XMMATRIX viewProjTranposedMatrix;

    void UpdateSpecific(double dt);
};

#endif
