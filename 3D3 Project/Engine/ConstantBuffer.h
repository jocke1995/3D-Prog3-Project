#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include "stdafx.h"

#include <set>

enum CONSTANT_BUFFER_TYPE
{
	CB_PER_OBJECT
};

struct CB_PER_OBJECT
{
	DirectX::XMFLOAT4X4 worldMatrix;
};

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
	ConstantBuffer(std::wstring name, unsigned int size, CONSTANT_BUFFER_TYPE type);
	~ConstantBuffer();

	bool SetData(void* location, const void* data, size_t size);
	void* GetValidLocation();
	ID3D12Resource1*& GetResource();

private:
	std::wstring name = nullptr;
	unsigned int size = 0;
	unsigned int entrySize = 0;

	ID3D12Resource1* constantBufferResource;

	std::set<unsigned int> validLocations;

};

#endif