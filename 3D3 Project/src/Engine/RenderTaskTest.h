#ifndef RENDERTASKTEST_H
#define RENDERTASKTEST_H

#include "RenderTask.h"

class RenderTaskTest : public RenderTask
{
public:
	RenderTaskTest(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds);
	~RenderTaskTest();

	void Execute(ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList5* commandList5, ID3D12RootSignature* rootSig, int backBufferIndex);

private:

	
};

#endif