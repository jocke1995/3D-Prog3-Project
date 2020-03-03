#ifndef DESCRIPTORHEAP_H
#define DESCRIPTORHEAP_H

#include "stdafx.h"

enum class DESCRIPTOR_HEAP_TYPE
{
	RTV,
	DSV,
	CBV_UAV_SRV
};

class DescriptorHeap
{
public:
	DescriptorHeap(ID3D12Device5* device, DESCRIPTOR_HEAP_TYPE type);
	~DescriptorHeap();

	D3D12_DESCRIPTOR_HEAP_DESC* GetDesc();
	ID3D12DescriptorHeap* GetID3D12DescriptorHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHeapAt(UINT backBufferIndex);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHeapAt(UINT backBufferIndex);
	UINT GetHandleIncrementSize();

	void SetCPUGPUHeapStart();
private:
	ID3D12DescriptorHeap* descriptorHeap = nullptr;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapStart;
	D3D12_GPU_DESCRIPTOR_HANDLE GPUHeapStart;

	D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapAt;
	D3D12_GPU_DESCRIPTOR_HANDLE GPUHeapAt;


	UINT handleIncrementSize;
};

#endif