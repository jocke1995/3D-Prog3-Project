#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include "RootSignature.h"
#include "RenderTask.h"
#include "SwapChain.h"

#include "ThreadPool.h"
#include "D3D12Timer.h"

#include "RenderTaskTest.h"
#include "RenderTaskBlend.h"

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

private:
	// Device
	ID3D12Device5* device5 = nullptr;

	bool CreateDevice();

	// CommandQueue
	ID3D12CommandQueue* commandQueue = nullptr;
	bool CreateCommandQueue();

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

	// RenderTasks
	std::vector<RenderTask*> renderTasks;
	std::vector<ID3D12CommandList*>listsToExecute[NUM_SWAP_BUFFERS];

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