#include "DX12Task.h"
#include "Resource.h"

DX12Task::DX12Task(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType)
{
	this->commandInterface = new CommandInterface(device, interfaceType);
}

DX12Task::~DX12Task()
{
	for (auto resource : this->resources)
	{
		ID3D12Resource* res = resource->GetID3D12Resource1();
		SAFE_RELEASE(&res);
	}

	delete this->commandInterface;
}

void DX12Task::AddResource(Resource* resource)
{
	this->resources.push_back(resource);
	resource->GetID3D12Resource1()->AddRef();
}

void DX12Task::SetBackBufferIndex(int backBufferIndex)
{
	this->backBufferIndex = backBufferIndex;
}

void DX12Task::SetCommandInterfaceIndex(int index)
{
	this->commandInterfaceIndex = index;
}

ID3D12GraphicsCommandList5* DX12Task::GetCommandList(unsigned int index)
{
	return this->commandInterface->GetCommandList(index);
}
