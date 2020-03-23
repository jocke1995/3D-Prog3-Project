#ifndef COPYTASK_H
#define COPYTASK_H

#include "DX12Task.h"
#include "stdafx.h"

class Resource;

class CopyTask : public DX12Task
{
public:
	CopyTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType, Resource *sourceResource, Resource* destinationResource);
	~CopyTask();

protected:
	Resource* sourceResource = nullptr;
	Resource* destinationResource = nullptr;
};
#endif
