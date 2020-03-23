#include "CopyTask.h"
#include "Resource.h"

CopyTask::CopyTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType, Resource* sourceResource, Resource* destinationResource)
	:DX12Task(device, interfaceType)
{
	this->sourceResource = sourceResource;
	this->destinationResource = destinationResource;
}

CopyTask::~CopyTask()
{
}
