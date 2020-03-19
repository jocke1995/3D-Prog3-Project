#ifndef RENDERTASKTEST_H
#define RENDERTASKTEST_H

#include "RenderTask.h"

class RenderTaskTest : public RenderTask
{
public:
	RenderTaskTest(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds);
	~RenderTaskTest();

	void Execute();

private:

	
};

#endif