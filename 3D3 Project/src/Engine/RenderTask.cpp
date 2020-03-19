#include "RenderTask.h"

RenderTask::RenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*> *gpsds)
{
	this->CreateCommandInterfaces(device);

	for (auto gpsd : *gpsds)
		this->pipelineStates.push_back(new PipelineState(device, rootSignature, VSName, PSName, gpsd));

	this->rootSig = rootSignature->GetRootSig();
}

RenderTask::~RenderTask()
{
	for (unsigned int i = 0; i < NUM_SWAP_BUFFERS; i++)
	{
		SAFE_RELEASE(&this->commandAllocators[i]);
		SAFE_RELEASE(&this->commandLists[i]);
	}
		

	for (auto pipelineState : this->pipelineStates)
		delete pipelineState;
}

PipelineState* RenderTask::GetPipelineState(unsigned int index)
{
	return this->pipelineStates[index];
}

ID3D12GraphicsCommandList* RenderTask::GetCommandList(unsigned int index)
{
	return this->commandLists[index];
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

void RenderTask::SetBackBufferIndex(int backBufferIndex)
{
	this->backBufferIndex = backBufferIndex;
}

void RenderTask::CreateCommandInterfaces(ID3D12Device5* device)
{
	for (UINT i = 0; i < NUM_SWAP_BUFFERS; i++)
	{
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&this->commandAllocators[i]));

		device->CreateCommandList(0, 
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			this->commandAllocators[i],
			nullptr,
			IID_PPV_ARGS(&this->commandLists[i]));

		this->commandLists[i]->Close();
	}
}

