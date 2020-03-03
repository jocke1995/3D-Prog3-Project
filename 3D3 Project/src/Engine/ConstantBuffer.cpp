#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(ID3D12Device5* device, std::wstring name, unsigned int size, CONSTANT_BUFFER_TYPE type)
{
	this->name = name;
	this->size = size;

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
	case CONSTANT_BUFFER_TYPE::CB_PER_OBJECT:
		this->entrySize = sizeof(CB_PER_OBJECT); // 16 float
	}

	resourceDesc.Width = size * entrySize;
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
		IID_PPV_ARGS(&this->constantBufferResource)
	);

	if (FAILED(hr))
	{
		OutputDebugStringW(L"ERROR: Failed to create Constant Buffer");
	}

	this->constantBufferResource->SetName(name.c_str());

	// Fill validLocations
	for (int i = 0; i < size; i++)
	{
		validLocations.insert(i);
	}

}

ConstantBuffer::~ConstantBuffer()
{
	// Make sure to not destory if still used on GPU
	SAFE_RELEASE(&constantBufferResource);
}

bool ConstantBuffer::SetData(void* location, const void* data, size_t entrySize)
{
	// TODO: return false if fail
	void* dataBegin = location;

	// Set up the heap data
	D3D12_RANGE range = { 0, 0 }; // We do not intend to read this resource on the CPU.

	constantBufferResource->Map(0, &range, &dataBegin); // Get a dataBegin pointer where we can copy data to
	memcpy(dataBegin, data, entrySize);
	constantBufferResource->Unmap(0, nullptr);

	return true;
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

ID3D12Resource1* ConstantBuffer::GetResource()
{
	return this->constantBufferResource;
}
