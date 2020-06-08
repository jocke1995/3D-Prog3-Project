#include "CopyColorTask.h"

CopyColorTask::CopyColorTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType)
	:CopyTask(device, interfaceType)
{
	
}

CopyColorTask::~CopyColorTask()
{
}

void CopyColorTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->commandInterfaceIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->commandInterfaceIndex);

	commandAllocator->Reset();
	commandList->Reset(commandAllocator, NULL);
	
	static float r = 0.0f;
	static float g = 0.0f;
	static float b = 0.0f;

	// For fun blinking color
	r += 0.0002f;
	g += 0.00035f;
	b += 0.0004f;

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

	commandList->Close();
}