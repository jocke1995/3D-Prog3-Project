#ifndef TRANSFORM_H
#define TRANSFORM_H

using namespace DirectX;

class Transform
{
public:
	Transform();
	~Transform();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 pos);
	
	void RotateX(float radians);
	void RotateY(float radians);
	void RotateZ(float radians);

	void SetScale(float scale);
	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3 scale);

	void UpdateWorldMatrix();

	const XMFLOAT4X4* GetWorldMatrix() const;
	XMFLOAT3 GetPositionXMFLOAT3() const;
	float3 GetPositionFloat3() const;

private:
	XMFLOAT3 position;
	XMFLOAT4X4 rotationMat;
	XMFLOAT3 scale;

	XMFLOAT4X4 worldMat;

	XMMATRIX rotXMat;
	XMMATRIX rotYMat;
	XMMATRIX rotZMat;

};

#endif