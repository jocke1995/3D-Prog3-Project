#ifndef RENDERTASK_H
#define RENDERTASK_H

#include "PipelineState.h"
#include "Object.h"
#include "RenderTarget.h"
#include "RootSignature.h"
#include "Camera.h"
#include "DepthBuffer.h"

class DescriptorHeap;

class RenderTask
{
public:
	RenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, D3D12_GRAPHICS_PIPELINE_STATE_DESC* gpsdTest);
	virtual ~RenderTask();

	PipelineState* GetPipelineState();

	void AddRenderTarget(RenderTarget* renderTarget);
	void AddObject(Object* object);

	void SetCamera(Camera* camera);
	void SetDescriptorHeap(DescriptorHeap* dh);
	void SetDepthBuffer(DepthBuffer* depthBuffer);

	virtual void Execute(ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList5* commandList5, ID3D12RootSignature* rootSig, int backBufferIndex) = 0;
private:

protected:
	DescriptorHeap* descriptorHeap = nullptr;

	std::vector<RenderTarget*> renderTargets;
	DepthBuffer* depthBuffer = nullptr;
	PipelineState* pipelineState = nullptr;
	std::vector<Object*> objects;
	Camera* camera = nullptr;
};

#endif