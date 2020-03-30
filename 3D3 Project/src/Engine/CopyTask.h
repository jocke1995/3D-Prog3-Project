#ifndef COPYTASK_H
#define COPYTASK_H

#include "DX12Task.h"

#include "Resource.h"

class CopyTask : public DX12Task
{
public:
	CopyTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType);
	~CopyTask();
};
#endif
