#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera()
{
	this->projMatrix = DirectX::XMMatrixOrthographicLH(800, 600, 0.1, 1000);
}

OrthographicCamera::~OrthographicCamera()
{
}

void OrthographicCamera::Update(double dt)
{
	this->viewProjMatrix = this->viewMatrix * this->projMatrix;
}

XMMATRIX* OrthographicCamera::GetViewProjMatrix()
{
	return &this->viewProjMatrix;
}
