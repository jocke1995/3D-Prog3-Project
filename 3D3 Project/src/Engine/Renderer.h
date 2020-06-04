#ifndef RENDERER_H
#define RENDERER_H

#include "RootSignature.h"
#include "RenderTask.h"
#include "SwapChain.h"
#include "ThreadPool.h"

// Graphics
#include "ForwardRenderTask.h"
#include "BlendRenderTask.h"

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
	void UpdateObjectsToDraw();

	void SortObjectsByDistance(XMFLOAT3 camPos);

	void SetCamera(Camera* camera);

	void Execute();

	ThreadPool* GetThreadPool();
private:
	unsigned int frameCounter = 0;

	// Device
	ID3D12Device5* device5 = nullptr;
	bool CreateDevice();

	// CommandQueues
	std::map<COMMAND_INTERFACE_TYPE, ID3D12CommandQueue*> commandQueues;
	void CreateCommandQueues();

	// Swapchain
	RenderTarget* swapChain = nullptr;
	void CreateSwapChain(HWND *hwnd);

	// Depthbuffer
	DepthBuffer* depthBuffer = nullptr;
	void CreateDepthBuffer();

	// Rootsignature
	RootSignature* rootSignature = nullptr;
	void CreateRootSignature();

	// ThreadPool
	ThreadPool* threadpool = nullptr;

	// Resource for the copyQueue
	Resource* copySourceResource = nullptr;
	Resource* copyDestResource = nullptr;

	// RenderTasks
	std::vector<RenderTask*>  renderTasks;
	std::vector<CopyTask*>    copyTasks;
	std::vector<ComputeTask*> computeTasks;

	// Objects to draw, a single vector holding all objects of different drawOptions
	std::vector<Object*> objectsToDraw;

	// Commandlists holders
	std::vector<ID3D12CommandList*> copyCommandLists[NUM_SWAP_BUFFERS];
	std::vector<ID3D12CommandList*> computeCommandLists[NUM_SWAP_BUFFERS];
	std::vector<ID3D12CommandList*> directCommandLists[NUM_SWAP_BUFFERS];
	
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