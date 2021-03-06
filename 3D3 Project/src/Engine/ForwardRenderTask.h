#ifndef FORWARDRENDERTASK_H
#define FORWARDRENDERTASK_H

#include "RenderTask.h"

class FowardRenderTask : public RenderTask
{
public:
	FowardRenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds, COMMAND_INTERFACE_TYPE interfaceType);
	~FowardRenderTask();

	void Execute();

private:
	
};

#endif