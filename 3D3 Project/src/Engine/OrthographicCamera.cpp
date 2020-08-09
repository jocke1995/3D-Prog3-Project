#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(
	float left,
	float right,
	float bot,
	float top,
	float nearZ,
	float farZ)
	:BaseCamera()
{
	this->projMatrix = DirectX::XMMatrixOrthographicOffCenterLH(left, right, bot, top, nearZ, farZ);
}
OrthographicCamera::~OrthographicCamera()
{
}

void OrthographicCamera::Update(double dt)
{
	this->viewProjMatrix = this->viewMatrix * this->projMatrix;
}

XMMATRIX* OrthographicCamera::GetViewProjection()
{
	return &this->viewProjMatrix;
}
