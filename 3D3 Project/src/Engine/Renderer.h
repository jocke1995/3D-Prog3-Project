#ifndef RENDERER_H
#define RENDERER_H

#include "RootSignature.h"
#include "RenderTask.h"
#include "SwapChain.h"

#include "ThreadPool.h"
#include "D3D12Timer.h"

// Graphics
#include "RenderTaskTest.h"
#include "RenderTaskBlend.h"

// Copy
#include "CopyColorTask.h"

// Compute
#include "ComputeTestTask.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void InitD3D12(HWND *hwnd);
	void InitRenderTasks();

	Mesh* CreateMesh(std::wstring path);

	void AddObjectToTasks(Object* object);

	void SetCamera(Camera* camera);

	void Execute();

	ThreadPool* GetThreadPool();
private:
	// Device
	ID3D12Device5* device5 = nullptr;
	bool CreateDevice();

	// CommandQueues
	std::map<COMMAND_INTERFACE_TYPE, ID3D12CommandQueue*> commandQueues;
	void CreateCommandQueues();

	// Swapchain
	RenderTarget* swapChain = nullptr;
	bool CreateSwapChain(HWND *hwnd);

	// ThreadPool
	ThreadPool* threadpool = nullptr;

	// Depthbuffer
	DepthBuffer* depthBuffer = nullptr;
	bool CreateDepthBuffer();

	// Rootsignature
	RootSignature* rootSignature = nullptr;
	bool CreateRootSignature();

	// Resource for the copyQueue
	Resource* copySourceResource;
	Resource* copyDestResource;

	// RenderTasks
	std::vector<RenderTask*> renderTasks;
	std::vector<CopyTask*> copyTasks;
	std::vector<ComputeTask*> computeTasks;

	// Commandlists holders
	std::vector<ID3D12CommandList*> directCommandLists[NUM_SWAP_BUFFERS];
	std::vector<ID3D12CommandList*> copyCommandLists[NUM_SWAP_BUFFERS];
	std::vector<ID3D12CommandList*> computeCommandLists[NUM_SWAP_BUFFERS];

	// DescriptorHeap
	DescriptorHeap* descriptorHeap = nullptr;
	void InitDescriptorHeap();

	// ShaderResourceView
	void CreateShaderResourceView(Mesh* mesh);

	// Fences
	HANDLE eventHandle = nullptr;
	ID3D12Fence1* fenceFrame = nullptr;
	UINT64 fenceFrameValue = 0;
	void CreateFences();
	void WaitForFrame();
};

#endif