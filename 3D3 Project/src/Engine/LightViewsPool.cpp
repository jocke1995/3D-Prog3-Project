#include "LightViewsPool.h"

LightViewsPool::LightViewsPool(
	ID3D12Device5* device,
	DescriptorHeap* descriptorHeap_CBV_UAV_SRV,
	DescriptorHeap* descriptorHeap_RTV,
	DescriptorHeap* descriptorHeap_DSV)
{
	this->device = device;

	this->descriptorHeap_CBV_UAV_SRV = descriptorHeap_CBV_UAV_SRV;
	this->descriptorHeap_RTV = descriptorHeap_RTV;
	this->descriptorHeap_DSV = descriptorHeap_DSV;
}

LightViewsPool::~LightViewsPool()
{
	for (int i = 0; i < LIGHT_TYPE::NUM_LIGHT_TYPES; i++)
	{
		LIGHT_TYPE typeIndex = static_cast<LIGHT_TYPE>(i);

		// CBVs
		for (auto& pair : this->cbvPools[typeIndex])
		{
			delete pair.second;
		}
	}
}

ConstantBufferView* LightViewsPool::GetFreeConstantBufferView(LIGHT_TYPE type)
{
	for (auto& pair : this->cbvPools[type])
	{
		// The resource is free
		if (pair.first == true)
		{
			pair.first = false;
			return pair.second;
		}
	}

	// No constant buffer of that type exists.. Create and return a new one
	ConstantBufferView* cbd = CreateConstantBufferView(type);
	this->cbvPools[type].push_back(std::make_pair(false, cbd));
	return cbd;
}

//ShadowInfo* LightViewsPool::GetFreeShadowInfo(LIGHT_TYPE type)
//{
	//for (auto& pair : this->shadowInfoPools[type])
	//{
	//	// The resource is free
	//	if (pair.first == true)
	//	{
	//		pair.first = false;
	//		return pair.second;
	//	}
	//}
	//
	//// No constant buffer of that type exists.. Create and return a new one
	//ShadowInfo* si = this->CreateShadowInfo(type);
	//this->shadowInfoPools[type].push_back(std::make_pair(false, si));
	//return si;
//}

void LightViewsPool::Clear()
{
	for (int i = 0; i < LIGHT_TYPE::NUM_LIGHT_TYPES; i++)
	{
		LIGHT_TYPE typeIndex = static_cast<LIGHT_TYPE>(i);

		// CBVs
		for (auto& pair : this->cbvPools[typeIndex])
		{
			pair.first = true;
		}
	}
}

ConstantBufferView* LightViewsPool::CreateConstantBufferView(LIGHT_TYPE type)
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

	ConstantBufferView* cbd = new ConstantBufferView(
		device,
		entrySize,
		resourceName,
		this->descriptorHeap_CBV_UAV_SRV->GetNextDescriptorHeapIndex(1),
		this->descriptorHeap_CBV_UAV_SRV);

	return cbd;
}
