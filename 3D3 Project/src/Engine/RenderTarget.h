#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "DescriptorHeap.h"
#include "Resource.h"

class RenderTarget
{
public:
	RenderTarget(ID3D12Device5* device, unsigned int width, unsigned int height, unsigned int nrOf = 1);
	virtual ~RenderTarget();
	RenderTarget() {}

	DescriptorHeap* GetDescriptorHeap();
	ID3D12Resource1* GetRenderTarget(UINT index);
	D3D12_VIEWPORT* GetViewPort();
	D3D12_RECT* GetScissorRect();

	

protected:
	void CreateViewport(unsigned int width, unsigned int height);
	void CreateScissorRect(unsigned int width, unsigned int height);
	
	DescriptorHeap* descriptorHeap = nullptr;

	std::vector<ID3D12Resource1*> resources;

	unsigned int width = 0;
	unsigned int height = 0;

	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
};

#endif