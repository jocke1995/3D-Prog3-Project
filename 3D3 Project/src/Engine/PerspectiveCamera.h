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
    PerspectiveCamera();
	virtual ~PerspectiveCamera();

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

    void Init();
    void UpdateCamera();
    void UpdateViewProjMatrix();

    //  ------------------------ TEMPORARY IN THIS CLASS ------------------------
    bool tempHasInputObject = false;
    TempInputClass* tempInputClass = nullptr;
};

#endif