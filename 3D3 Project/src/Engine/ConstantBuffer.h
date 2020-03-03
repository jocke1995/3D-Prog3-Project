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

class ConstantBuffer
{
public:
	ConstantBuffer(ID3D12Device5* device, std::wstring name, unsigned int size, CONSTANT_BUFFER_TYPE type);
	~ConstantBuffer();

	bool SetData(void* location, const void* data, size_t entrySize);
	void* GetValidLocation();
	ID3D12Resource1* GetResource();

private:
	std::wstring name;
	unsigned int size = 0;
	unsigned int entrySize = 0;

	ID3D12Resource1* constantBufferResource;

	std::set<unsigned int> validLocations;

};

#endif