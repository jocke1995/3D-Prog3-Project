#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(std::wstring name) : name(name)
{
}

ConstantBuffer::~ConstantBuffer()
{
	// Make sure to not destory if still used on GPU
	constantBufferResource->Release();
}

bool ConstantBuffer::SetData(const void* data, size_t size)
{
	// TODO: return false if fail
	void* dataBegin = nullptr;

	// Set up the heap data
	D3D12_RANGE range = { 0, 0 }; // We do not intend to read this resource on the CPU.

	constantBufferResource->Map(0, &range, &dataBegin); // Get a dataBegin pointer where we can copy data to
	memcpy(dataBegin, data, size);
	constantBufferResource->Unmap(0, nullptr);

	return true;
}

ID3D12Resource1*& ConstantBuffer::GetResource()
{
	return this->constantBufferResource;
}
