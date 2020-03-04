#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include "stdafx.h"

#include <set>

enum CONSTANT_BUFFER_TYPE
{
	CB_PER_OBJECT,
	CB_CAMERA
};



class ConstantBuffer
{
public:
	ConstantBuffer(ID3D12Device5* device, std::wstring name, unsigned int nrEntries, CONSTANT_BUFFER_TYPE type);
	~ConstantBuffer();

	bool SetData(void* beginLocation, const void* data);
	CONSTANT_BUFFER_TYPE GetType();
	void* GetValidLocation();
	ID3D12Resource1* GetResource();

private:
	std::wstring name;
	CONSTANT_BUFFER_TYPE type;

	unsigned int nrEntries = 0;
	unsigned int entrySize = 0;

	ID3D12Resource1* constantBufferResource;

	std::set<unsigned int> validLocations;

};

#endif