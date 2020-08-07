#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "BaseCamera.h"

class OrthographicCamera : public BaseCamera
{
public:
    OrthographicCamera();
    virtual ~OrthographicCamera();

    void Update(double dt);

    XMMATRIX* GetViewProjMatrix();

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
