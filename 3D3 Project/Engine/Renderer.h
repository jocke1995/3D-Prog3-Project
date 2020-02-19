#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"
#include "RootSignature.h"

#define NUM_SWAP_BUFFERS 2

class Renderer
{
public:
	Renderer();
	~Renderer();

	void CreateWindow(HINSTANCE hInstance, int nCmdShow, int screenWidth, int screenHeight, bool fullScreen, LPCTSTR windowName, LPCTSTR windowTitle);
	void InitD3D12();

	void Execute();

private:
	Window * window = nullptr;

	// Device
	ID3D12Device5* device5 = nullptr;
	bool CreateDevice();

	// CommandQueue
	ID3D12CommandQueue* commandQueue = nullptr;

	// Swapchain
	IDXGISwapChain4* swapChain4 = nullptr;
	bool CreateSwapChain();

	// Rootsignature
	RootSignature* rootSignature = nullptr;
};

#endif