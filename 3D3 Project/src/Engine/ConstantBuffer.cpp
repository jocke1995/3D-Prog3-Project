#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(std::wstring name, unsigned int size, unsigned int entrySize) : name(name), size(size), entrySize(entrySize)
{
	// This will be set later by the renderer
	this->constantBufferResource = nullptr;

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
