#ifndef DEPTHSTENCILVIEW_H
#define DEPTHSTENCILVIEW_H

#include "DescriptorHeap.h"
#include "Resource.h"

class DepthStencilView
{
public:
	DepthStencilView(
		ID3D12Device5* device,
		unsigned int width, unsigned int height,
		DescriptorHeap* descriptorHeap_DSV);
	virtual ~DepthStencilView();

private:

	Resource* resource = nullptr;

	void CreateResource(ID3D12Device5* device, unsigned int width, unsigned int height);
	void CreateDSV(ID3D12Device5* device, DescriptorHeap* descriptorHeap_DSV);
};

#endif