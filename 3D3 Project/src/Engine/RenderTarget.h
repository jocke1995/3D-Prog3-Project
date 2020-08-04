#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "DescriptorHeap.h"
#include "Resource.h"

class RenderTarget
{
public:
	// Use when creating renderTargets
	RenderTarget(
		ID3D12Device5* device,
		unsigned int width, unsigned int height,
		DescriptorHeap* descriptorHeap_RTV);

	virtual ~RenderTarget();

	Resource* GetResource(unsigned int index) const;
	const D3D12_VIEWPORT* GetViewPort() const;
	const D3D12_RECT* GetScissorRect() const;

private:
	std::vector<Resource*> resources;

	unsigned int width = 0;
	unsigned int height = 0;

	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
	void CreateViewport();
	void CreateScissorRect();
};

#endif