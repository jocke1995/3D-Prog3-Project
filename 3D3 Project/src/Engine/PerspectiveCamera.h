#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "TempInputClass.h"

#include "BaseCamera.h"

class PerspectiveCamera : public BaseCamera
{
public:
    // Temp constructor for input (USED ONCE ATM)
    PerspectiveCamera(HINSTANCE hInstance, HWND hwnd);

    // main constructor
    PerspectiveCamera(XMVECTOR position, XMVECTOR lookAt);
	virtual ~PerspectiveCamera();

    XMMATRIX* GetViewProjection();
    XMMATRIX* GetViewProjectionTranposed();

private:
    float moveLeftRight = 0.0f;
    float moveForwardBackward = 0.0f;
    float moveUpDown = 0.0f;
    float camYaw = 0.0f;
    float camPitch = 0.0f;

    XMMATRIX projMatrix;

    XMMATRIX viewProjMatrix;
    XMMATRIX viewProjTranposedMatrix;

    void Init();

    void UpdateSpecific(double dt);
    void UpdateCameraMovement();

    //  ------------------------ TEMPORARY IN THIS CLASS ------------------------
    bool tempHasInputObject = false;
    TempInputClass* tempInputClass = nullptr;
};

#endif