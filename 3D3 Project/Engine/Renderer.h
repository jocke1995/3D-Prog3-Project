#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"
#include "RootSignature.h"
#include "RenderTask.h"

#define NUM_SWAP_BUFFERS 2

class Renderer
{
public:
	Renderer();
	~Renderer();

	void CreateWindow(HINSTANCE hInstance, int nCmdShow, int screenWidth, int screenHeight, bool fullScreen, LPCTSTR windowName, LPCTSTR windowTitle);
	void InitD3D12();

	void CreateRenderTask(RenderTask* renderTask);
	void AddRenderTask(RenderTask * renderTask);

	void Execute();

private:
	Window * window = nullptr;

	// Device
	ID3D12Device5* device5 = nullptr;
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
	bool CreateSwapChain();

	// Rootsignature
	RootSignature* rootSignature = nullptr;

	// RenderTasks
	std::vector<RenderTask*> renderTasks;
	bool CreatePSO(RenderTask* renderTask);
};

#endif