#include "Computetask.h"

ComputeTask::ComputeTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType)
	:DX12Task(device, interfaceType)
{

}

ComputeTask::~ComputeTask()
{
}
