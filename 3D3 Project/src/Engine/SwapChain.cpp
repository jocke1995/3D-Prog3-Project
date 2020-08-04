#include "SwapChain.h"

SwapChain::SwapChain(
	ID3D12Device5* device,
	const HWND* hwnd,
	unsigned int width, unsigned int height,
	ID3D12CommandQueue* commandQueue,
	DescriptorHeap* descriptorHeap_RTV)
{
	// Init resources (make sure they exist)
	this->dx12Resources.resize(NUM_SWAP_BUFFERS);
	this->width = width;
	this->height = height;

	IDXGIFactory4* factory = nullptr;
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));

	if (hr != S_OK)
	{
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to create DXGIFactory for SwapChain\n");
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
	else
	{
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to create Swapchain\n");
	}

	SAFE_RELEASE(&factory);

	// Connect the renderTargets to the swapchain, so that the swapchain can easily swap between these two renderTargets
	for (unsigned int i = 0; i < NUM_SWAP_BUFFERS; i++)
	{
		HRESULT hr = swapChain4->GetBuffer(i, IID_PPV_ARGS(&this->dx12Resources[i]));
		if (FAILED(hr))
		{
			Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to GetBuffer from RenderTarget to Swapchain\n");
		}

		unsigned int dhIndex = descriptorHeap_RTV->GetNextDescriptorHeapIndex(1);
		D3D12_CPU_DESCRIPTOR_HANDLE cdh = descriptorHeap_RTV->GetCPUHeapAt(dhIndex);
		device->CreateRenderTargetView(this->dx12Resources[i], nullptr, cdh);
	}

	this->CreateViewport();
	this->CreateScissorRect();
}

SwapChain::~SwapChain()
{
	SAFE_RELEASE(&this->swapChain4);

	for (unsigned int i = 0; i < NUM_SWAP_BUFFERS; i++)
	{
		SAFE_RELEASE(&this->dx12Resources[i]);
	}
}

IDXGISwapChain4* SwapChain::GetDX12SwapChain() const
{
	return this->swapChain4;
}

ID3D12Resource1* SwapChain::GetDX12Resource(unsigned int index) const
{
	return this->dx12Resources[index];
}

const D3D12_VIEWPORT* SwapChain::GetViewPort() const
{
	return &this->viewport;
}

const D3D12_RECT* SwapChain::GetScissorRect() const
{
	return &this->scissorRect;
}

void SwapChain::CreateViewport()
{
	this->viewport.TopLeftX = 0.0f;
	this->viewport.TopLeftY = 0.0f;
	this->viewport.Width = (float)this->width;
	this->viewport.Height = (float)this->height;
	this->viewport.MinDepth = 0.0f;
	this->viewport.MaxDepth = 1.0f;
}

void SwapChain::CreateScissorRect()
{
	this->scissorRect.left = (long)0;
	this->scissorRect.right = (long)this->width;
	this->scissorRect.top = (long)0;
	this->scissorRect.bottom = (long)this->height;
}
