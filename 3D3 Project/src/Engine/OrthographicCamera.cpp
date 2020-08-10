#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(XMVECTOR position, XMVECTOR lookAt, float left, float right, float bot, float top, float nearZ, float farZ)
	:BaseCamera(position, lookAt)
{
	this->projMatrix = DirectX::XMMatrixOrthographicOffCenterLH(left, right, bot, top, nearZ, farZ);
}
OrthographicCamera::~OrthographicCamera()
{
}

void OrthographicCamera::UpdateSpecific(double dt)
{
	this->viewProjMatrix = this->viewMatrix * this->projMatrix;
}

XMMATRIX* OrthographicCamera::GetViewProjection()
{
	return &this->viewProjMatrix;
}
