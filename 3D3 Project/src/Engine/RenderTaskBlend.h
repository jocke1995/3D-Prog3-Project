#ifndef RENDERTASKBLEND_H
#define RENDERTASKBLEND_H

#include "RenderTask.h"

class RenderTaskBlend : public RenderTask
{
public:
	RenderTaskBlend(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds, COMMAND_INTERFACE_TYPE interfaceType);
	~RenderTaskBlend();

	void Execute();

private:

};

#endif