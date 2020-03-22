#include "CopyColorTask.h"

CopyColorTask::CopyColorTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds, COMMAND_INTERFACE_TYPE cqType)
	:RenderTask(device, rootSignature, VSName, PSName, gpsds, cqType)
{

}

CopyColorTask::~CopyColorTask()
{
}

//extern D3D12::D3D12Timer timer;
void CopyColorTask::Execute()
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
