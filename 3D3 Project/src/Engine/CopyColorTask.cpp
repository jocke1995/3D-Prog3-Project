#include "CopyColorTask.h"
#include "Resource.h"

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

	//D3D12_RESOURCE_BARRIER rb;

	float4 red = { 1.0f, 0.0f, 0.0f, 1.0f };
	this->sourceResource->SetData(&red);

	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->destinationResource->GetID3D12Resource1(),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_COMMON));

	commandList->CopyResource(this->destinationResource->GetID3D12Resource1(),
							  this->sourceResource->GetID3D12Resource1());

	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->destinationResource->GetID3D12Resource1(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_GENERIC_READ));

	// End timestamp
	//timer.stop(commandList, timer_index);
	//timer.resolveQueryToCPU(commandList, timer_index);

	commandList->Close();
}