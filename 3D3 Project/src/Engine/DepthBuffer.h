#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include "stdafx.h"
#include "DescriptorHeap.h"

class DepthBuffer
{
public:
	DepthBuffer(ID3D12Device5* device, unsigned int width, unsigned int height);
	virtual ~DepthBuffer();

	DescriptorHeap* GetDescriptorHeap();

	D3D12_VIEWPORT* GetViewPort();
	D3D12_RECT* GetScissorRect();

protected:
	void CreateViewport(unsigned int width = 800, unsigned int height = 600);
	void CreateScissorRect(unsigned int width = 800, unsigned int height = 600);

	DescriptorHeap* descriptorHeap = nullptr;
	ID3D12Resource1* resource = nullptr;

	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
};

#endif