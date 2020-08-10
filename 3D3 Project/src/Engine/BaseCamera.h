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
	BaseCamera(XMVECTOR position = { 0.0, 3.0, -5.0, 1.0f }, XMVECTOR lookAt = { 0.0f, 0.0f, 1.0f, 1.0f });
	virtual ~BaseCamera();

	void Update(double dt);

	void SetPosition(float x, float y, float z);
	void SetLookAt(float x, float y, float z);

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

	virtual void UpdateSpecific(double dt = 0.0) = 0;
};

#endif