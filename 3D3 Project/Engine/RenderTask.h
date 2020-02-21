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

	D3D12_GRAPHICS_PIPELINE_STATE_DESC* Getgpsd();
private:

	PipelineState* pipelineState = nullptr;

	// All specific rendertasks needs to fill out the description of the PSO.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = {};
};

#endif