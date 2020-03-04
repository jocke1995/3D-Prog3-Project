#ifndef RENDERTASK_H
#define RENDERTASK_H

#include "PipelineState.h"
#include "Object.h"
#include "RenderTarget.h"
#include "RootSignature.h"
#include "Camera.h"

class RenderTask
{
public:
	RenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, D3D12_GRAPHICS_PIPELINE_STATE_DESC* gpsdTest);
	virtual ~RenderTask();

	PipelineState* GetPipelineState();

	void AddRenderTarget(RenderTarget* renderTarget);
	void AddConstantBuffer(ConstantBuffer* cb);

	void SetObjectsToDraw(std::vector<Object*> *objects);
	void SetCamera(Camera* camera);
	void SetDescriptorHeap(DescriptorHeap* dh);

	virtual void Execute(ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList5* commandList5, ID3D12RootSignature* rootSig, int backBufferIndex) = 0;
private:

protected:
	DescriptorHeap* descriptorHeap = nullptr;
	std::map<CONSTANT_BUFFER_TYPE, ConstantBuffer*> constantBuffers;

	std::vector<RenderTarget*> renderTargets;
	PipelineState* pipelineState = nullptr;
	std::vector<Object*> *objects = nullptr;
	Camera* camera = nullptr;
};

#endif