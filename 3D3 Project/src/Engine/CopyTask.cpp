#include "CopyTask.h"
#include "Resource.h"

CopyTask::CopyTask(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType)
	:DX12Task(device, interfaceType)
{

}

CopyTask::~CopyTask()
{
}
