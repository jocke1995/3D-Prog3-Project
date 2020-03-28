#ifndef COMPUTETASK_H
#define COMPUTETASK_H

#include "DX12Task.h"

class ComputeTask : public DX12Task
{
public:
	ComputeTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType);
	~ComputeTask();
};
#endif
