#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "DescriptorHeap.h"

class RenderTarget
{
public:
	RenderTarget(ID3D12Device5* device);
	virtual ~RenderTarget();

	DescriptorHeap* GetDescriptorHeap();
	ID3D12Resource1* GetRenderTarget(UINT index);
	D3D12_VIEWPORT* GetViewPort();
	D3D12_RECT* GetScissorRect();

	

protected:
	void CreateViewport(unsigned int width = 800, unsigned int height = 600);
	void CreateScissorRect(unsigned int width = 800, unsigned int height = 600);
	
	DescriptorHeap* descriptorHeap = nullptr;

	ID3D12Resource1* renderTargets[NUM_SWAP_BUFFERS] = {};

	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
};

#endif