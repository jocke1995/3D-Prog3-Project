#include "ComputeTask.h"

ComputeTask::ComputeTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR CSName, LPCTSTR psoName, COMMAND_INTERFACE_TYPE interfaceType)
	:DX12Task(device, interfaceType)
{
	this->pipelineState = new ComputeState(device, rootSignature, CSName, psoName);

	this->rootSig = rootSignature->GetRootSig();
}

ComputeTask::~ComputeTask()
{
	delete this->pipelineState;
}
