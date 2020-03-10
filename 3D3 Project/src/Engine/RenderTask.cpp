#include "RenderTask.h"

RenderTask::RenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, D3D12_GRAPHICS_PIPELINE_STATE_DESC* gpsdTest)
{
	this->pipelineState = new PipelineState(device, rootSignature, VSName, PSName, gpsdTest);
}

RenderTask::~RenderTask()
{
	delete this->pipelineState;
}

PipelineState* RenderTask::GetPipelineState()
{
	return this->pipelineState;
}

void RenderTask::AddRenderTarget(RenderTarget* renderTarget)
{
	this->renderTargets.push_back(renderTarget);
}

void RenderTask::AddObject(Object* object)
{
	this->objects.push_back(object);
}

void RenderTask::SetCamera(Camera* camera)
{
	this->camera = camera;
}

void RenderTask::SetDescriptorHeap(DescriptorHeap* dh)
{
	this->descriptorHeap = dh;
}

void RenderTask::SetDepthBuffer(DepthBuffer* depthBuffer)
{
	this->depthBuffer = depthBuffer;
}

