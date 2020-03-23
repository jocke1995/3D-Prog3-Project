#pragma once

#include "stdafx.h"
#include "Task.h"
#include "CommandInterface.h"

class Resource;

class DX12Task : public Task
{
public:
	DX12Task(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType);
	~DX12Task();

	void AddResource(Resource* resource);
	void SetBackBufferIndex(int backBufferIndex);

	ID3D12GraphicsCommandList5* GetCommandList(unsigned int index);
protected:
	std::vector<Resource*> resources;

	CommandInterface* commandInterface = nullptr;
	int backBufferIndex = -1;
};