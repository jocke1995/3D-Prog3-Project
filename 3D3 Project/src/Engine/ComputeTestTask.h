#ifndef COMPUTETESTTASK_H
#define COMPUTETESTTASK_H

#include "ComputeTask.h"

class ComputeTestTask : public ComputeTask
{
public:
	ComputeTestTask(ID3D12Device5* device,
		RootSignature* rootSignature,
		LPCWSTR CSName,
		LPCTSTR psoName,
		COMMAND_INTERFACE_TYPE interfaceType);

	~ComputeTestTask();

	void Execute();

};

#endif