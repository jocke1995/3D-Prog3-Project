#include "DescriptorHeap.h"

DescriptorHeap::DescriptorHeap(ID3D12Device5* device, DESCRIPTOR_HEAP_TYPE type)
{
	// Create description
	switch (type)
	{
	case DESCRIPTOR_HEAP_TYPE::RTV:
		this->desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		this->desc.NumDescriptors = NUM_SWAP_BUFFERS;
		break;
	case DESCRIPTOR_HEAP_TYPE::DSV:
		this->desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		this->desc.NumDescriptors = NUM_SWAP_BUFFERS;
		break;
	case DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV:
		this->desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		this->desc.NumDescriptors = 100000;
		this->desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		break;
	}



	this->handleIncrementSize = device->GetDescriptorHandleIncrementSize(this->desc.Type);

	// Create descriptorHeap for the renderTarget
	
	HRESULT hr = device->CreateDescriptorHeap(&this->desc, IID_PPV_ARGS(&this->descriptorHeap));
	if (hr != S_OK)
	{
		OutputDebugStringA("Error: Failed to create DescriptorHeap For SwapChainRenderTarget!\n");
	}

	this->SetCPUGPUHeapStart();
}

DescriptorHeap::~DescriptorHeap()
{
	SAFE_RELEASE(&this->descriptorHeap);
}

D3D12_DESCRIPTOR_HEAP_DESC* DescriptorHeap::GetDesc()
{
	return &this->desc;
}

ID3D12DescriptorHeap* DescriptorHeap::GetID3D12DescriptorHeap()
{
	return this->descriptorHeap;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUHeapAt(UINT descriptorIndex)
{
	CPUHeapAt.ptr = CPUHeapStart.ptr + this->handleIncrementSize * descriptorIndex;
	return CPUHeapAt;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUHeapAt(UINT descriptorIndex)
{
	GPUHeapAt.ptr = GPUHeapStart.ptr + this->handleIncrementSize * descriptorIndex;
	return GPUHeapAt;
}

UINT DescriptorHeap::GetHandleIncrementSize()
{
	return this->handleIncrementSize;
}

void DescriptorHeap::SetCPUGPUHeapStart()
{
	this->CPUHeapStart = this->descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	this->GPUHeapStart = this->descriptorHeap->GetGPUDescriptorHandleForHeapStart();
}
