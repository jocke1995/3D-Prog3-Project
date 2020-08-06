#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "DescriptorHeap.h"
#include "Resource.h"

class RenderTarget
{
public:
	// RenderTarget which creates a comitted Resource
	RenderTarget(
		ID3D12Device5* device,
		unsigned int width, unsigned int height,
		DescriptorHeap* descriptorHeap_RTV);

	// RenderTarget without creating a comittedResource
	RenderTarget(unsigned int width, unsigned int height);

	virtual ~RenderTarget();

	Resource* GetResource(unsigned int index) const;
	const D3D12_VIEWPORT* GetViewPort() const;
	const D3D12_RECT* GetScissorRect() const;

protected:
	std::vector<Resource*> resources;

	unsigned int width = 0;
	unsigned int height = 0;

private:
	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
	void CreateViewport();
	void CreateScissorRect();
};

#endif