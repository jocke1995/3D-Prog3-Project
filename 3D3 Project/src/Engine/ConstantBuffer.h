#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include "stdafx.h"
#include "Resource.h"
#include <set>

enum CONSTANT_BUFFER_TYPE
{
	CB_PER_OBJECT_TYPE,
	CB_CAMERA_TYPE
};

class ConstantBuffer : public Resource
{
public:
	ConstantBuffer(ID3D12Device5* device, unsigned int nrEntries, CONSTANT_BUFFER_TYPE type, std::wstring name = L"CB_NONAME" );
	~ConstantBuffer();

	bool SetData(void* beginLocation, const void* data);
	CONSTANT_BUFFER_TYPE GetType();
	void* GetValidLocation();

private:
	CONSTANT_BUFFER_TYPE type;
	unsigned int nrEntries = 0;

	std::set<unsigned int> validLocations;

};

#endif