#include "Computetask.h"

ComputeTask::ComputeTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR CSName, COMMAND_INTERFACE_TYPE interfaceType)
	:DX12Task(device, interfaceType)
{
	this->pipelineState = new PipelineState(device, rootSignature, CSName);

	this->rootSig = rootSignature->GetRootSig();
}

ComputeTask::~ComputeTask()
{
	delete this->pipelineState;
}
