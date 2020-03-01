#include "DescriptorHeap.h"

DescriptorHeap::DescriptorHeap(DESCRIPTOR_HEAP_TYPES type, UINT handleIncrementSize)
{
	this->Init(type, handleIncrementSize);
}

DescriptorHeap::~DescriptorHeap()
{
	SAFE_RELEASE(&this->descriptorHeap);
}

D3D12_DESCRIPTOR_HEAP_DESC* DescriptorHeap::GetDesc()
{
	return &this->desc;
}

ID3D12DescriptorHeap** DescriptorHeap::GetID3D12DescriptorHeap()
{
	return &this->descriptorHeap;
}

D3D12_CPU_DESCRIPTOR_HANDLE* DescriptorHeap::GetCPUHeapAt(UINT backBufferIndex)
{
	CPUHeapAt.ptr = CPUHeapStart.ptr + this->handleIncrementSize * backBufferIndex;
	return &CPUHeapAt;
}

D3D12_GPU_DESCRIPTOR_HANDLE* DescriptorHeap::GetGPUHeapAt(UINT backBufferIndex)
{
	GPUHeapAt.ptr = GPUHeapStart.ptr + this->handleIncrementSize * backBufferIndex;
	return &GPUHeapAt;
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

void DescriptorHeap::Init(DESCRIPTOR_HEAP_TYPES type, UINT handleIncrementSize)
{
	this->desc.NumDescriptors = NUM_SWAP_BUFFERS;

	switch (type)
	{
	case DESCRIPTOR_HEAP_TYPES::RTV:
		this->desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		break;
	case DESCRIPTOR_HEAP_TYPES::DSV:
		this->desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		break;
	case DESCRIPTOR_HEAP_TYPES::CBV_UAV_SRV:
		this->desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		break;
	}

	this->handleIncrementSize = handleIncrementSize;
}
