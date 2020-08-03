#include "CopyTask.h"
#include "Resource.h"

CopyTask::CopyTask(ID3D12Device5* device)
	:DX12Task(device, COMMAND_INTERFACE_TYPE::COPY_TYPE)
{

}

CopyTask::~CopyTask()
{
}

void CopyTask::AddDataToUpdate(std::pair<void*, ConstantBufferDefault*>* data_CBD)
{
	this->data_CBDs.push_back(*data_CBD);
}

void CopyTask::Clear()
{
	this->data_CBDs.clear();
}
