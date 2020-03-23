#pragma once

#include "stdafx.h"
#include "Task.h"
#include "CommandInterface.h"

class DX12Task : public Task
{
public:
	DX12Task(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType);
	~DX12Task();

	void SetBackBufferIndex(int backBufferIndex);

protected:
	CommandInterface* commandInterface = nullptr;
	int backBufferIndex = -1;
};