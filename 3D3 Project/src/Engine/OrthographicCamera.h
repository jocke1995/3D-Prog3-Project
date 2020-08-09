#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "BaseCamera.h"

class OrthographicCamera : public BaseCamera
{
public:
    OrthographicCamera(
        float left = -15.0f,
        float right = 15.0f,
        float bot = -15.0f,
        float top = 15.0f,
        float nearZ = -1.0f,
        float farZ = 10.0f);
    virtual ~OrthographicCamera();

    void Update(double dt);

    XMMATRIX* GetViewProjection();

private:
    float moveLeftRight = 0.0f;
    float moveForwardBackward = 0.0f;
    float moveUpDown = 0.0f;
    float camYaw = 0.0f;
    float camPitch = 0.0f;

    XMMATRIX projMatrix;
    XMMATRIX viewProjMatrix;
};

#endif
