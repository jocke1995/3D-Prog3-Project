#include "Resource.h"

Resource::Resource(ID3D12Device* device, unsigned long long entrySize, RESOURCE_TYPE type, std::wstring name)
{
	this->name = name;
	this->entrySize = entrySize;

	D3D12_HEAP_TYPE d3d12HeapType;
	D3D12_RESOURCE_STATES startState = D3D12_RESOURCE_STATE_GENERIC_READ;
	switch (type)
	{
	case RESOURCE_TYPE::UPLOAD:
		d3d12HeapType = D3D12_HEAP_TYPE_UPLOAD;
		break;
	case RESOURCE_TYPE::DEFAULT:
		d3d12HeapType = D3D12_HEAP_TYPE_DEFAULT;
		break;
	case RESOURCE_TYPE::RESOURCE_COPY:
		d3d12HeapType = D3D12_HEAP_TYPE_DEFAULT;
		startState = D3D12_RESOURCE_STATE_COMMON;
		break;
	}

	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = d3d12HeapType;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.CreationNodeMask = 1; //used when multi-gpu
	heapProperties.VisibleNodeMask = 1; //used when multi-gpu
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = entrySize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		startState,
		nullptr,
		IID_PPV_ARGS(&this->resource)
	);

	if (FAILED(hr))
	{
		OutputDebugStringW(L"ERROR: Failed to create Resource\n");
	}

	this->resource->SetName(name.c_str());
}

Resource::Resource(std::wstring name)
{
	this->name = name;
}

Resource::~Resource()
{
	SAFE_RELEASE(&this->resource);
}

size_t Resource::GetSize() const
{
	return this->entrySize;
}

ID3D12Resource1* Resource::GetID3D12Resource1() const
{
	return this->resource;
}

D3D12_GPU_VIRTUAL_ADDRESS Resource::GetGPUVirtualAdress() const
{
	return this->resource->GetGPUVirtualAddress();
}

void Resource::SetData(const void* data)
{
	void* dataBegin = nullptr;

	// Set up the heap data
	D3D12_RANGE range = { 0, 0 }; // We do not intend to read this resource on the CPU.

	this->resource->Map(0, &range, &dataBegin); // Get a dataBegin pointer where we can copy data to
	memcpy(dataBegin, data, this->entrySize);
	this->resource->Unmap(0, nullptr);
}
