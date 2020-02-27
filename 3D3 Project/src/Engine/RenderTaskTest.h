#ifndef RENDERTASKTEST_H
#define RENDERTASKTEST_H

#include "RenderTask.h"


class RenderTaskTest : public RenderTask
{
public:
	RenderTaskTest();
	~RenderTaskTest();

	void Execute(ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList5* commandList5, ID3D12RootSignature* rootSig, int backBufferIndex);

private:
	void SPECIFY_GRAPHICS_PIPELINE_STATE_DESC();
};

#endif