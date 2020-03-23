#include "CopyColorTask.h"

CopyColorTask::CopyColorTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType, Resource* sourceResource, Resource* destinationResource)
	:CopyTask(device, interfaceType, sourceResource, destinationResource)
{

}

CopyColorTask::~CopyColorTask()
{
}


//extern D3D12::D3D12Timer timer;
void CopyColorTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->backBufferIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->backBufferIndex);

	commandAllocator->Reset();
	commandList->Reset(commandAllocator, NULL);

	// Start timestamp
	//UINT timer_index = 1;
	//timer.start(commandList, timer_index);


	//source.SetData();
	//commandList.CopyResource(source, dest);

	// End timestamp
	//timer.stop(commandList, timer_index);
	//timer.resolveQueryToCPU(commandList, timer_index);

	commandList->Close();
}