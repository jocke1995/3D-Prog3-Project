#ifndef LIGHTCONSTANTBUFFERPOOL_H
#define LIGHTCONSTANTBUFFERPOOL_H

#include "ConstantBufferDefault.h"
#include "../ECS/Components/Lights/Light.h"

class LightConstantBufferPool
{
public:
	LightConstantBufferPool(ID3D12Device5* device, DescriptorHeap* descriptorHeap_CBV_UAV_SRV);
	~LightConstantBufferPool();

	ConstantBufferDefault* GetFreeConstantBufferDefault(LIGHT_TYPE type);

	void FreeConstantBuffers();
private:
	std::map<LIGHT_TYPE, std::vector<std::pair<bool, ConstantBufferDefault*>>> constantBufferPools;
	ID3D12Device5* device = nullptr;
	DescriptorHeap* descriptorHeap_CBV_UAV_SRV = nullptr;

	ConstantBufferDefault* CreateConstantBufferDefault(LIGHT_TYPE type);
};

#endif
