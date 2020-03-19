#ifndef RENDERTASK_H
#define RENDERTASK_H

#include "PipelineState.h"
#include "Object.h"
#include "RenderTarget.h"
#include "RootSignature.h"
#include "Camera.h"
#include "DepthBuffer.h"
#include "D3D12Timer.h"
#include "Task.h"

class DescriptorHeap;

class RenderTask : public Task
{
public:
	RenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*> *gpsdTest);
	virtual ~RenderTask();

	PipelineState* GetPipelineState(unsigned int index);
	ID3D12GraphicsCommandList* GetCommandList(unsigned int index);

	void AddRenderTarget(RenderTarget* renderTarget);
	void AddObject(Object* object);

	void SetCamera(Camera* camera);
	void SetDescriptorHeap(DescriptorHeap* dh);
	void SetDepthBuffer(DepthBuffer* depthBuffer);
	void SetBackBufferIndex(int backBufferIndex);
private:
	void CreateCommandInterfaces(ID3D12Device5* device);

protected:
	DescriptorHeap* descriptorHeap = nullptr;

	std::vector<RenderTarget*> renderTargets;
	DepthBuffer* depthBuffer = nullptr;
	int backBufferIndex = -1;
	ID3D12RootSignature* rootSig = nullptr;

	ID3D12GraphicsCommandList5* commandLists[NUM_SWAP_BUFFERS];
	ID3D12CommandAllocator* commandAllocators[NUM_SWAP_BUFFERS];
	
	std::vector<PipelineState*> pipelineStates;
	std::vector<Object*> objects;
	Camera* camera = nullptr;
};

#endif