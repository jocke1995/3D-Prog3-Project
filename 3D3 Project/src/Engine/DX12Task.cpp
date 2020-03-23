#include "DX12Task.h"

DX12Task::DX12Task(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType)
{
	this->commandInterface = new CommandInterface(device, interfaceType);
}

DX12Task::~DX12Task()
{
	delete this->commandInterface;
}

void DX12Task::SetBackBufferIndex(int backBufferIndex)
{
	this->backBufferIndex = backBufferIndex;
}
