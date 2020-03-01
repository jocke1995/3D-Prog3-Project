#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include "RootSignature.h"
#include "RenderTask.h"
#include "ConstantBuffer.h"

#include "RenderTaskTest.h"

// Create a different class for each type of renderTarget, then specify which type to use when creating one
enum RenderTargetTypes
{
	SWAPCHAIN,
	RENDERTARGET,	// Rendertargets which allocates their own memory.. Ex: Deferred Rendering - Position,Normal..   TODO: NAMN
	DEPTH
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void InitD3D12(HWND *hwnd);

	// Create Constant Buffer
	ConstantBuffer* CreateConstantBuffer(std::wstring name, D3D12_HEAP_TYPE heapType, unsigned int size, CONSTANT_BUFFER_TYPE type);
	// Create Vertex Buffer
	void CreateVertexBuffer(Mesh* mesh);

	void AddRenderTask(RenderTask * renderTask);

	void Execute();

	RenderTarget* GetRenderTarget(RenderTargetTypes rtt, int index);

private:

	// Device
	ID3D12Device5* device5 = nullptr;

	std::vector<ConstantBuffer*> constantBuffers; // TODO:: NUM_BUFFERS Buffering

	bool CreateDevice();

	// CommandQueue
	ID3D12CommandQueue* commandQueue = nullptr;
	bool CreateCommandQueue();

	// TEMPORARY, JUST TO GET IT TO WORK WITH 1 THREAD:
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList5* commandList5 = nullptr;
	void CreateAllocatorAndListTemporary();

	// Swapchain
	IDXGISwapChain4* swapChain4 = nullptr;
	bool CreateSwapChain(HWND *hwnd);

	// Rootsignature
	RootSignature* rootSignature = nullptr;
	bool CreateRootSignature();

	// RenderTasks
	std::vector<RenderTask*> renderTasks;
	bool CreatePSO(RenderTask* renderTask);

	std::map<RenderTargetTypes, std::vector<RenderTarget*>> renderTargetsHolder;
	// The order of the renderTargets in the vector
	enum RenderTargets
	{
		RT_SwapChain
	};
	bool CreateRenderTarget(RenderTargetTypes rtt);

	


	// Fences
	ID3D12Fence1* fence = nullptr;
	HANDLE eventHandle = nullptr;
	UINT64 fenceValue = 0;
	void TempCreateFence();
	void WaitForGPU();
};

#endif