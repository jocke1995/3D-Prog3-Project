#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "stdafx.h"

class Transform
{
public:
	Transform();
	~Transform();


	SetPos();
	SetRot();
	SetScale();

	GetPos();
	GetRot();
	GetScale();
	GetWorldMatrix();

	ConstantBuffer& GetConstantBuffer();
private:
	
	Mat4 WorldMatrix;

	ConstantBuffer& worldConstantBuffer;


	UpdateWorldMatrix();



    
};

#endif