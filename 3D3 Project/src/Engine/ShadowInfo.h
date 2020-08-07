#ifndef SHADOWINFO_H
#define SHADOWINFO_H

#include "DepthStencilView.h"
#include "ShaderResourceView.h"

class ShadowInfo
{
public:
	ShadowInfo(
		unsigned int textureWidth, unsigned int textureHeight,
		ID3D12Device5* device,
		DescriptorHeap* dh_DSV,
		DescriptorHeap* dh_SRV);

	virtual ~ShadowInfo();

private:
	Resource* resource = nullptr;

	DepthStencilView* DSV = nullptr;
	ShaderResourceView* SRV = nullptr;

	void CreateResource(ID3D12Device5* device, unsigned int width, unsigned int height);
	void CreateDSV(ID3D12Device5* device, DescriptorHeap* dh_DSV);
	void CreateSRV(ID3D12Device5* device, DescriptorHeap* dh_SRV);
};

#endif