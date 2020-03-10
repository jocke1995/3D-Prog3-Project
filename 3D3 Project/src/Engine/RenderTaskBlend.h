#ifndef RENDERTASKBLEND_H
#define RENDERTASKBLEND_H

#include "RenderTask.h"

class RenderTaskBlend : public RenderTask
{
public:
	RenderTaskBlend(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, D3D12_GRAPHICS_PIPELINE_STATE_DESC* gpsdTest);
	~RenderTaskBlend();

	void Execute(ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList5* commandList5, ID3D12RootSignature* rootSig, int backBufferIndex);

private:
};

#endif