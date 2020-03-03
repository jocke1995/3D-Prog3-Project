#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include "RootSignature.h"
#include "RenderTask.h"
#include "ConstantBuffer.h"
#include "SwapChain.h"

#include "RenderTaskTest.h"

// Create a different class for each type of renderTarget, then specify which type to use when creating one
enum RenderTargetTypes
{
	SWAPCHAIN,
	RENDERTARGET,	// Rendertargets which allocates their own memory.. Ex: Deferred Rendering - Position,Normal..   TODO: NAMN
	DEPTH
};

enum RenderTaskType
{
	TEST
};

enum ConstantBufferIndex
{
	CB_TRANSFORM
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void InitD3D12(HWND *hwnd);
	void InitRenderTasks();

	// Create Constant Buffer
	ConstantBuffer* CreateConstantBuffer(std::wstring name, unsigned int size, CONSTANT_BUFFER_TYPE type);
	// Create Vertex Buffer
	void CreateVertexBuffer(Mesh* mesh);

	void SetObjectsToDraw(RenderTaskType type, std::vector<Object*> *objects);

	void Execute();

	ConstantBuffer* GetConstantBuffer(ConstantBufferIndex index);

private:

	// Device
	ID3D12Device5* device5 = nullptr;

	std::map<ConstantBufferIndex, ConstantBuffer*> constantBuffers; // TODO:: NUM_BUFFERS Buffering

	bool CreateDevice();

	// CommandQueue
	ID3D12CommandQueue* commandQueue = nullptr;
	bool CreateCommandQueue();

	// TEMPORARY, JUST TO GET IT TO WORK WITH 1 THREAD:
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList5* commandList5 = nullptr;
	void CreateAllocatorAndListTemporary();

	// Swapchain
	bool CreateSwapChain(HWND *hwnd);
	RenderTarget* swapChain = nullptr;

	// Rootsignature
	RootSignature* rootSignature = nullptr;
	bool CreateRootSignature();

	// RenderTasks
	std::map<RenderTaskType, RenderTask*> renderTasks;

	// The order of the renderTargets in the vector

	// DescriptorHeap
	


	// Fences
	ID3D12Fence1* fence = nullptr;
	HANDLE eventHandle = nullptr;
	UINT64 fenceValue = 0;
	void TempCreateFence();
	void WaitForGPU();
};

#endif