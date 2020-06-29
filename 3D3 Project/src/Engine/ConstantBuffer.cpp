#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(ID3D12Device5* device, unsigned int nrEntries, CONSTANT_BUFFER_TYPE type, std::wstring name)
	: Resource(name) 
{
	this->type = type;
	this->nrEntries = nrEntries;

	UINT cbSizeAligned = (sizeof(CB_PER_ENTITY)* nrEntries + 255) & ~255;	// 256-byte aligned CB.

	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.CreationNodeMask = 1; //used when multi-gpu
	heapProperties.VisibleNodeMask = 1; //used when multi-gpu
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	switch (type)
	{
	case CONSTANT_BUFFER_TYPE::CB_PER_ENTITY_TYPE:
		this->entrySize = sizeof(CB_PER_ENTITY);
	}

	resourceDesc.Width = cbSizeAligned;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->resource)
	);

	if (FAILED(hr))
	{
		std::string cbName(this->name.begin(), this->name.end());
		Log::PrintError(Log::ErrorType::ENGINE, "Failed to create constant buffer with name: \'%s\'\n", cbName.c_str());
	}

	this->resource->SetName(name.c_str());

	// Fill validLocations
	for (int i = 0; i < nrEntries; i++)
	{
		validLocations.insert(i);
	}

}

ConstantBuffer::~ConstantBuffer()
{
	
}

void ConstantBuffer::SetData(void* beginLocation, const void* data)
{
	void* dataBegin = beginLocation;

	// Set up the heap data
	D3D12_RANGE range = { 0, 0 }; // We do not intend to read this resource on the CPU.

	this->resource->Map(0, &range, &dataBegin); // Get a dataBegin pointer where we can copy data to
	memcpy(dataBegin, data, this->entrySize);
	this->resource->Unmap(0, nullptr);
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGPUAt(UINT index) const
{
	if (index > this->entrySize)
	{
		std::string cbName(this->name.begin(), this->name.end());
		Log::PrintError(Log::ErrorType::ENGINE, "\'%s\' Memory Out of Range\n", cbName.c_str());
		return 0;
	}

	D3D12_GPU_VIRTUAL_ADDRESS address = this->resource->GetGPUVirtualAddress();
	address = (index * 256) + address;
	return address;
}

CONSTANT_BUFFER_TYPE ConstantBuffer::GetType() const
{
	return this->type;
}

void* ConstantBuffer::GetValidLocation()
{
	void* location = nullptr;

	if (!this->validLocations.empty())
	{
		// it is a (index of an entry) number between 0 and size of the constant buffer
		auto firstValid = this->validLocations.begin();

		unsigned int address = entrySize * (*firstValid);

		location = (void*)(address);

		// remove it from the set
		this->validLocations.erase(firstValid);
	}

	return location;
}