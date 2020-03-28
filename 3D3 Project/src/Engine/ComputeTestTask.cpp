#include "ComputeTestTask.h"

ComputeTestTask::ComputeTestTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR CSName, COMMAND_INTERFACE_TYPE interfaceType)
	:ComputeTask(device, rootSignature, CSName, interfaceType)
{
}

ComputeTestTask::~ComputeTestTask()
{
}

//extern D3D12::D3D12Timer timer;
void ComputeTestTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->backBufferIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->backBufferIndex);

	commandAllocator->Reset();
	commandList->Reset(commandAllocator, NULL);

	// Start timestamp
	//UINT timer_index = 1;
	//timer.start(commandList, timer_index);
	
	commandList->SetComputeRootSignature(this->rootSig);

	commandList->SetPipelineState(this->pipelineState->GetPSO());

	commandList->Dispatch(1, 1, 1);

	// End timestamp
	//timer.stop(commandList, timer_index);
	//timer.resolveQueryToCPU(commandList, timer_index);

	commandList->Close();
}
