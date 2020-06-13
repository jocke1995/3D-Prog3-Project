#include "ComputeTestTask.h"

ComputeTestTask::ComputeTestTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR CSName, COMMAND_INTERFACE_TYPE interfaceType)
	:ComputeTask(device, rootSignature, CSName, interfaceType)
{
}

ComputeTestTask::~ComputeTestTask()
{
}

void ComputeTestTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->commandInterfaceIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->commandInterfaceIndex);

	commandAllocator->Reset();
	commandList->Reset(commandAllocator, NULL);
	
	commandList->SetComputeRootSignature(this->rootSig);

	commandList->SetPipelineState(this->pipelineState->GetPSO());

	// Resource with color from the copyQueue -> this ComputeQueue -> DirectQueue
	commandList->SetComputeRootUnorderedAccessView(RS::ColorUAV, 
		this->resources[0]->GetGPUVirtualAdress());

	commandList->Dispatch(1, 1, 1);

	commandList->Close();
}
