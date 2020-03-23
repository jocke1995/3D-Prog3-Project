#pragma once

#include "stdafx.h"

class CommandInterface
{
public:
	CommandInterface(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType);
	~CommandInterface();

	ID3D12GraphicsCommandList5* GetCommandList(int index);
	ID3D12CommandAllocator* GetCommandAllocator(int index);

private:
	ID3D12GraphicsCommandList5* commandLists[NUM_SWAP_BUFFERS];
	ID3D12CommandAllocator* commandAllocators[NUM_SWAP_BUFFERS];

	void CreateCommandInterfaces(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType);
};