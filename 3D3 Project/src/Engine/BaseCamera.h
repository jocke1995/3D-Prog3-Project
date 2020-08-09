#ifndef BASECAMERA_H
#define BASECAMERA_H

using namespace DirectX;

enum CAMERA_TYPE
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
	NUM_CAMERA_TYPES
};
class BaseCamera
{
public:
	BaseCamera();
	virtual ~BaseCamera();

	virtual void Update(double dt = 0.0) = 0;
	void SetPosition(float x, float y, float z);

	XMFLOAT3 GetPosition() const;
	float3 GetPositionFloat3() const;

	XMFLOAT3 GetLookAt() const;
	float3 GetLookAtFloat3() const;

	virtual XMMATRIX* GetViewProjection() = 0;

protected:
	XMVECTOR rightVector;
	XMVECTOR eyeVector;
	XMVECTOR atVector;
	XMVECTOR upVector;

	XMMATRIX viewMatrix;
};

#endif