#include "RenderTask.h"

RenderTask::RenderTask()
{
	this->pipelineState = new PipelineState();
}

RenderTask::~RenderTask()
{
	delete this->pipelineState;
}

PipelineState* RenderTask::GetPipelineState()
{
	return this->pipelineState;
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC* RenderTask::Getgpsd()
{
	return &this->gpsd;
}
