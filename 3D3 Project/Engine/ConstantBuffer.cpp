#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(std::wstring name, unsigned int size, CONSTANT_BUFFER_TYPE type) : name(name), size(size)
{
	// This will be set later by the renderer
	this->constantBufferResource = nullptr;

	switch (type)
	{
	case CONSTANT_BUFFER_TYPE::CB_PER_OBJECT: 
		entrySize = sizeof(CB_PER_OBJECT); // 16 float
	}

	// Fill validLocations
	for (int i = 0; i < size; i++)
	{
		validLocations.insert(i);
	}
}

ConstantBuffer::~ConstantBuffer()
{
	// Make sure to not destory if still used on GPU
	constantBufferResource->Release();
}

bool ConstantBuffer::SetData(void* location, const void* data, size_t size)
{
	// TODO: return false if fail
	void* dataBegin = location;

	// Set up the heap data
	D3D12_RANGE range = { 0, 0 }; // We do not intend to read this resource on the CPU.

	constantBufferResource->Map(0, &range, &dataBegin); // Get a dataBegin pointer where we can copy data to
	memcpy(dataBegin, data, size);
	constantBufferResource->Unmap(0, nullptr);

	return true;
}

void* ConstantBuffer::GetValidLocation()
{
	void* location = nullptr;

	if (!this->validLocations.empty())
	{
		// it is a (index of an entry) number between 0 and size of the constant buffer
		auto it = this->validLocations.begin();

		unsigned int address = entrySize * (*it);

		location = (void*)(address);

		// remove it from the set
		this->validLocations.erase(it);
	}

	return location;
}

ID3D12Resource1*& ConstantBuffer::GetResource()
{
	return this->constantBufferResource;
}
