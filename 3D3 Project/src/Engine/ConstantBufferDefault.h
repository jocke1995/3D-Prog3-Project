#ifndef CONSTANTBUFFERDEFAULT_H
#define CONSTANTBUFFERDEFAULT_H

#include "ConstantBuffer.h"

class ConstantBufferDefault : public ConstantBuffer
{
public:
	ConstantBufferDefault(ID3D12Device5* device,
		unsigned int entrySize,
		std::wstring defaultName,
		unsigned int descriptorHeapIndex,
		DescriptorHeap* descriptorHeap_CBV_UAV_SRV);

	virtual ~ConstantBufferDefault();

	Resource* GetDefaultResource() const;
	
private:
	Resource* defaultResource = nullptr;

	void CreateConstantBufferView(
		ID3D12Device5* device,
		DescriptorHeap* descriptorHeap_CBV_UAV_SRV);
};

#endif
