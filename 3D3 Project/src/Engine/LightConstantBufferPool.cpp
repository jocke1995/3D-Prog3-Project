#include "LightConstantBufferPool.h"

extern unsigned int globalDescriptorHeapIndex;
LightConstantBufferPool::LightConstantBufferPool(ID3D12Device5* device, DescriptorHeap* descriptorHeap_CBV_UAV_SRV)
{
	this->device = device;
	this->descriptorHeap_CBV_UAV_SRV = descriptorHeap_CBV_UAV_SRV;
}

LightConstantBufferPool::~LightConstantBufferPool()
{
	for (int i = 0; i < LIGHT_TYPE::NUM_LIGHT_TYPES; i++)
	{
		LIGHT_TYPE typeIndex = static_cast<LIGHT_TYPE>(i);
		for (auto& pair : this->constantBufferPools[typeIndex])
		{
			delete pair.second;
		}
	}
}

ConstantBufferDefault* LightConstantBufferPool::GetFreeConstantBufferDefault(LIGHT_TYPE type)
{
	for (auto& pair : this->constantBufferPools[type])
	{
		// The resource is free
		if (pair.first == true)
		{
			pair.first = false;
			return pair.second;
		}
	}

	// No constant buffer of that type exists.. Create and return a new one
	ConstantBufferDefault* cbd = CreateConstantBufferDefault(type);
	this->constantBufferPools[type].push_back(std::make_pair(false, cbd));
	return cbd;
}

void LightConstantBufferPool::FreeConstantBuffers()
{
	for (int i = 0; i < LIGHT_TYPE::NUM_LIGHT_TYPES; i++)
	{
		LIGHT_TYPE typeIndex = static_cast<LIGHT_TYPE>(i);
		for (auto& pair : this->constantBufferPools[typeIndex])
		{
			pair.first = true;
		}
	}
}

ConstantBufferDefault* LightConstantBufferPool::CreateConstantBufferDefault(LIGHT_TYPE type)
{
	unsigned int entrySize = 0;
	std::wstring resourceName = L"";
	switch (type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		entrySize = (sizeof(DirectionalLight) + 255) & ~255;	// align to 255-byte boundary
		resourceName = L"DirectionalLight_DefaultResource";
		break;
	case LIGHT_TYPE::POINT_LIGHT:
		entrySize = (sizeof(PointLight) + 255) & ~255;	// align to 255-byte boundary
		resourceName = L"PointLight_DefaultResource";
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		entrySize = (sizeof(SpotLight) + 255) & ~255;	// align to 255-byte boundary
		resourceName = L"SpotLight_DefaultResource";
		break;
	}

	ConstantBufferDefault* cbd = new ConstantBufferDefault(
		device,
		entrySize,
		resourceName,
		globalDescriptorHeapIndex++,
		this->descriptorHeap_CBV_UAV_SRV);

	return cbd;
}
