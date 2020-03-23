#ifndef COPYCOLORTASK_H
#define COPYCOLORTASK_H

#include "CopyTask.h"

class CopyColorTask : public CopyTask
{
public:
	CopyColorTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType);
	~CopyColorTask();

	void Execute();

};

#endif