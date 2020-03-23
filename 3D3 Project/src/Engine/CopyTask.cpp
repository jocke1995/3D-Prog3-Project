#include "CopyTask.h"

CopyTask::CopyTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType)
	:RenderTask(device, interfaceType)
{

}

CopyTask::~CopyTask()
{
}

//extern D3D12::D3D12Timer timer;
void CopyTask::Execute()
{
	this->commandAllocators[this->backBufferIndex]->Reset();
	this->commandLists[this->backBufferIndex]->Reset(this->commandAllocators[this->backBufferIndex], NULL);

	// Start timestamp
	//UINT timer_index = 1;
	//timer.start(this->commandLists[this->backBufferIndex], timer_index);

	this->commandLists[this->backBufferIndex]->SetGraphicsRootSignature(this->rootSig);

	//source.SetData();
	//commandList.CopyResource(source, dest);

	// End timestamp
	//timer.stop(this->commandLists[this->backBufferIndex], timer_index);
	//timer.resolveQueryToCPU(this->commandLists[this->backBufferIndex], timer_index);

	this->commandLists[this->backBufferIndex]->Close();
}
