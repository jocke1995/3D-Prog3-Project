#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include "stdafx.h"

struct xyzw
{
	float x;
	float y;
	float z;
	float w;
};

struct rgba
{
	float r;
	float g;
	float b;
	float a;
};

struct cbufferFloat4
{
	union
	{
		xyzw position;
		rgba color;
	};
};

class ConstantBuffer
{
public:
	ConstantBuffer(std::wstring name);
	~ConstantBuffer();

	bool SetData(const void* data, size_t size);
	ID3D12Resource1*& GetResource();

private:
	// TODO: add more relevant data such as width, height? Stefan???
	std::wstring name = nullptr;
	ID3D12Resource1* constantBufferResource = nullptr;



};

#endif