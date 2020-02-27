#ifndef RENDERTASK_H
#define RENDERTASK_H

#include "PipelineState.h"
//#include "Object.h"
#include "RenderTarget.h"


class RenderTask
{
public:
	RenderTask();
	~RenderTask();

	PipelineState* GetPipelineState();
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetGpsd();

	void AddRenderTarget(RenderTarget* renderTarget);

	virtual void Execute(ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList5* commandList5, ID3D12RootSignature* rootSig, int backBufferIndex) = 0;
private:

	
	// All specific rendertasks needs to fill out the description of the PSO.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = {};
	virtual void SPECIFY_GRAPHICS_PIPELINE_STATE_DESC() = 0;

protected:
	std::vector<RenderTarget*> renderTargets;
	PipelineState* pipelineState = nullptr;
};

#endif