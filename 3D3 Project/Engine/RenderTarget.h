#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "stdafx.h"

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	void SetRTDescriptorSize(UINT size);

	UINT GetRTDescriptorSize();
	ID3D12DescriptorHeap** GetRTHeap();
	ID3D12Resource1** GetRenderTarget(UINT index);
	D3D12_VIEWPORT* GetViewPort();
	D3D12_RECT* GetScissorRect();

	void CreateViewport(unsigned int width = 800, unsigned int height = 600);
	void CreateScissorRect(unsigned int width = 800, unsigned int height = 600);

private:

	ID3D12DescriptorHeap* renderTargetsHeap = nullptr;
	UINT renderTargetDescriptorSize = 0;

	ID3D12Resource1* renderTargets[NUM_SWAP_BUFFERS] = {};

	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
};

#endif