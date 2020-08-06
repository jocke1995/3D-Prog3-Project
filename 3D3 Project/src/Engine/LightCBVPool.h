#ifndef LIGHTCBVPOOL_H
#define LIGHTCBVPOOL_H

#include "ConstantBufferView.h"
#include "../ECS/Components/Lights/Light.h"

class LightCBVPool
{
public:
	LightCBVPool(ID3D12Device5* device, DescriptorHeap* descriptorHeap_CBV_UAV_SRV);
	~LightCBVPool();

	ConstantBufferView* GetFreeConstantBufferDefault(LIGHT_TYPE type);

	void FreeConstantBuffers();
private:
	std::map<LIGHT_TYPE, std::vector<std::pair<bool, ConstantBufferView*>>> constantBufferPools;
	ID3D12Device5* device = nullptr;
	DescriptorHeap* descriptorHeap_CBV_UAV_SRV = nullptr;

	ConstantBufferView* CreateConstantBufferDefault(LIGHT_TYPE type);
};

#endif
