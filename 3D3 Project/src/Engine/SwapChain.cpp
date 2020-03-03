#include "SwapChain.h"

SwapChain::SwapChain(ID3D12Device5* device, HWND* hwnd, ID3D12CommandQueue* commandQueue)
	: RenderTarget(device)
{

	IDXGIFactory4* factory = nullptr;
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));

	if (hr != S_OK)
	{
		OutputDebugStringA("Error: Failed to create DXGIFactory!\n");
	}

	//Create descriptor
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = 0;
	scDesc.Height = 0;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.Stereo = FALSE;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = NUM_SWAP_BUFFERS;
	scDesc.Scaling = DXGI_SCALING_NONE;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.Flags = 0;
	scDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	IDXGISwapChain1* swapChain1 = nullptr;
	if (SUCCEEDED(factory->CreateSwapChainForHwnd(
		commandQueue,
		*hwnd,
		&scDesc,
		nullptr,
		nullptr,
		&swapChain1)))
	{
		if (SUCCEEDED(swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain4))))
		{
			swapChain4->Release();
		}
	}



	SAFE_RELEASE(&factory);

	D3D12_CPU_DESCRIPTOR_HANDLE cdh = this->descriptorHeap->GetCPUHeapAt(0);

	// Connect the renderTargets to the swapchain, so that the swapchain can easily swap between these two renderTargets
	for (UINT i = 0; i < NUM_SWAP_BUFFERS; i++)
	{
		HRESULT hr = swapChain4->GetBuffer(i, IID_PPV_ARGS(&this->renderTargets[i]));
		if (hr != S_OK)
		{
			OutputDebugStringA("Error: Failed to GetBuffer from RenderTarget to Swapchain!\n");
		}

		device->CreateRenderTargetView(this->renderTargets[i], nullptr, cdh);
		cdh.ptr += this->descriptorHeap->GetHandleIncrementSize();
	}
}

SwapChain::~SwapChain()
{
	SAFE_RELEASE(&this->swapChain4);
}

IDXGISwapChain4* SwapChain::GetDX12SwapChain()
{
	return this->swapChain4;
}
