#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include "DescriptorHeap.h"

class DepthBuffer
{
public:
	DepthBuffer(ID3D12Device5* device, unsigned int width, unsigned int height);
	virtual ~DepthBuffer();

	DescriptorHeap* GetDescriptorHeap() const;
	const D3D12_VIEWPORT* GetViewPort() const;
	const D3D12_RECT* GetScissorRect() const;

protected:
	void CreateViewport(unsigned int width, unsigned int height);
	void CreateScissorRect(unsigned int width, unsigned int height);

	DescriptorHeap* descriptorHeap = nullptr;
	ID3D12Resource1* resource = nullptr;
	unsigned int width = 0;
	unsigned int height = 0;


	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
};

#endif