#ifndef COPYTASK_H
#define COPYTASK_H

#include "RenderTask.h"

class CopyTask : public RenderTask
{
public:
	CopyTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType);
	~CopyTask();

	void Execute();

private:
	Resource* destination;
};

#endif