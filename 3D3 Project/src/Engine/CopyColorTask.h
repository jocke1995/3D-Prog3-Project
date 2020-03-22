#ifndef COPYCOLORTASK_H
#define COPYCOLORTASK_H

#include "RenderTask.h"

class CopyColorTask : public RenderTask
{
public:
	CopyColorTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds, COMMAND_INTERFACE_TYPE cqType);
	~CopyColorTask();

	void Execute();

private:
	Resource* destination;
};

#endif