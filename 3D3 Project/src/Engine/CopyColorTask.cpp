#include "CopyColorTask.h"
#include "Resource.h"

CopyColorTask::CopyColorTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType)
	:CopyTask(device, interfaceType)
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
	
	static float r = 0.0f;
	static float g = 0.0f;
	static float b = 0.0f;

	// For fun blinking color
	r += 0.0002f;
	g += 0.00035f;
	b += 0.0004f;

	/* Done in ComputeShader now
	float testr = abs(sinf(r));
	float testg = abs(sinf(g));
	float testb = abs(sinf(b));
	*/


	float4 color = { r, g, b, 1.0 };

	this->resources[0]->SetData(&color);

	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->resources[1]->GetID3D12Resource1(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_COPY_DEST));

	commandList->CopyResource(this->resources[1]->GetID3D12Resource1(),
							  this->resources[0]->GetID3D12Resource1());

	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->resources[1]->GetID3D12Resource1(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_COMMON));

	// End timestamp
	//timer.stop(commandList, timer_index);
	//timer.resolveQueryToCPU(commandList, timer_index);

	commandList->Close();
}