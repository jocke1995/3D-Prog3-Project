#include "RenderTask.h"

RenderTask::RenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*> *gpsds)
{
	for (auto gpsd : *gpsds)
		this->pipelineStates.push_back(new PipelineState(device, rootSignature, VSName, PSName, gpsd));
}

RenderTask::~RenderTask()
{
	for (auto pipelineState : this->pipelineStates)
		delete pipelineState;
}

PipelineState* RenderTask::GetPipelineState(unsigned int index)
{
	return this->pipelineStates[index];
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

