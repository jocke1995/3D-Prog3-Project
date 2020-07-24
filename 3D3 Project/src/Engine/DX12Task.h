#pragma once

#include "Task.h"
#include "CommandInterface.h"

#include "D3D12Timer.h"
#include "D3D12TimerCopy.h"

class Resource;

enum RENDER_TASK_TYPE
{
	FORWARD_RENDER,
	BLEND,
	NR_OF_RENDERTASKS
};

enum COMPUTE_TASK_TYPE
{
	NR_OF_COMPUTETASKS
};

enum COPY_TASK_TYPE
{
	NR_OF_COPYTASKS
};

class DX12Task : public Task
{
public:
	DX12Task(ID3D12Device5* device, COMMAND_INTERFACE_TYPE interfaceType);
	~DX12Task();

	void AddResource(Resource* resource);
	void SetBackBufferIndex(int backBufferIndex);
	void SetCommandInterfaceIndex(int index);

	ID3D12GraphicsCommandList5* GetCommandList(unsigned int index) const;
protected:
	std::vector<Resource*> resources;

	CommandInterface* commandInterface = nullptr;
	int backBufferIndex = -1;
	int commandInterfaceIndex = -1;
};