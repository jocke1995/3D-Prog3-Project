#include "RenderTask.h"

RenderTask::RenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds, LPCTSTR psoName, COMMAND_INTERFACE_TYPE interfaceType)
	:DX12Task(device, interfaceType)
{
	for (auto gpsd : *gpsds)
		this->pipelineStates.push_back(new GraphicsState(device, rootSignature, VSName, PSName, gpsd, psoName));

	this->rootSig = rootSignature->GetRootSig();
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

void RenderTask::SetEntitiesToDraw(std::vector<Entity*> *entitiesToDraw)
{
	this->entities = *entitiesToDraw;
}

void RenderTask::AddEntity(Entity* entity)
{
	this->entities.push_back(entity);
}

void RenderTask::SetCamera(Camera* camera)
{
	this->camera = camera;
}

void RenderTask::SetDescriptorHeap_CBV_UAV_SRV(DescriptorHeap* dh)
{
	this->descriptorHeap_CBV_UAV_SRV = dh;
}

void RenderTask::SetDepthBuffer(DepthBuffer* depthBuffer)
{
	this->depthBuffer = depthBuffer;
}
