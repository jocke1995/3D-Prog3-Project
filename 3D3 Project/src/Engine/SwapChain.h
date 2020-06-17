#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "RenderTarget.h"

class SwapChain : public RenderTarget
{
public:
	SwapChain(ID3D12Device5* device, const HWND* hwnd, ID3D12CommandQueue* commandQueue);
	~SwapChain();
	

	IDXGISwapChain4* GetDX12SwapChain() const;

private:
	IDXGISwapChain4* swapChain4 = nullptr;
};

#endif