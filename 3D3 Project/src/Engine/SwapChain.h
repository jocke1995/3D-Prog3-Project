#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "DescriptorHeap.h"

class SwapChain
{
public:
	SwapChain(
		ID3D12Device5* device,
		const HWND* hwnd,
		unsigned int width, unsigned int height,
		ID3D12CommandQueue* commandQueue,
		DescriptorHeap* descriptorHeap_RTV);
	~SwapChain();

	IDXGISwapChain4* GetDX12SwapChain() const;
	ID3D12Resource1* GetDX12Resource(unsigned int index) const;

	const D3D12_VIEWPORT* GetViewPort() const;
	const D3D12_RECT* GetScissorRect() const;
private:
	IDXGISwapChain4* swapChain4 = nullptr;

	std::vector<ID3D12Resource1*> dx12Resources;

	unsigned int width = 0;
	unsigned int height = 0;

	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
	void CreateViewport();
	void CreateScissorRect();
};

#endif