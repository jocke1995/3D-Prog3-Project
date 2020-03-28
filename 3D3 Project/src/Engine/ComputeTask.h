#ifndef COMPUTETASK_H
#define COMPUTETASK_H

#include "DX12Task.h"
#include "PipelineState.h"

class ComputeTask : public DX12Task
{
public:
	ComputeTask(ID3D12Device5* device,
		RootSignature* rootSignature,
		LPCWSTR CSName,
		COMMAND_INTERFACE_TYPE interfaceType);
	~ComputeTask();

private:
	ID3D12RootSignature* rootSig = nullptr;

	PipelineState* pipelineState = nullptr;
};
#endif
