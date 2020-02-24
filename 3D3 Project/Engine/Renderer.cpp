#include "Renderer.h"

Renderer::Renderer()
{
	// Nothing here yet
}

Renderer::~Renderer()
{
	for (auto rts1 : this->renderTargetsHolder)
		for (auto rts2 : rts1.second)
			delete rts2;

	delete this->window;
	delete this->rootSignature;
}

void Renderer::CreateWindow(HINSTANCE hInstance, int nCmdShow, int screenWidth, int screenHeight, bool fullScreen, LPCTSTR windowName, LPCTSTR windowTitle)
{
	this->window = new Window(hInstance, nCmdShow, screenWidth, screenHeight, fullScreen, windowName, windowTitle);
}

void Renderer::InitD3D12()
{
	// Create Device
	if (!this->CreateDevice())
	{
		OutputDebugStringA("Error: Failed to create Device!\n");
	}

	// Create CommandQueue
	if (!this->CreateCommandQueue())
	{
		OutputDebugStringA("Error: Failed to create CommandQueue!\n");
	}

	// TEMP
	this->CreateAllocatorAndListTemporary();

	// Create SwapChain
	if (!this->CreateSwapChain())
	{
		OutputDebugStringA("Error: Failed to create SwapChain!\n");
	}

	// Create Rootsignature
	if (!this->CreateRootSignature())
	{
		OutputDebugStringA("Error: Failed to create SwapChain!\n");
	}

	// Create RenderTasks (SPECIFIC FOR KIND OF GAME LATER ON)
	if (!this->CreateRenderTarget(RenderTargetTypes::SWAPCHAIN))
	{
		OutputDebugStringA("Error: Failed to create RenderTarget!\n");
	}
}

void Renderer::AddRenderTask(RenderTask* renderTask)
{
	this->CreatePSO(renderTask);
	this->renderTasks.push_back(renderTask);
}

void Renderer::Execute()
{
	while (!this->window->ExitWindow())
	{
		// Här inne ska vi fylla commandolistorna senare
	}
}

RenderTarget* Renderer::GetRenderTarget(RenderTargetTypes rtt, int index)
{
	return this->renderTargetsHolder[rtt].at(index);
}

// -----------------------  Private Functions  ----------------------- //

bool Renderer::CreateDevice()
{
	bool deviceCreated = false;

#ifdef _DEBUG
	//Enable the D3D12 debug layer.
	ID3D12Debug* debugController = nullptr;

#ifdef STATIC_LINK_DEBUGSTUFF
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
	SafeRelease(debugController);
#else
	HMODULE mD3D12 = LoadLibrary(L"D3D12.dll"); // istället för GetModuleHandle

	PFN_D3D12_GET_DEBUG_INTERFACE f = (PFN_D3D12_GET_DEBUG_INTERFACE)GetProcAddress(mD3D12, "D3D12GetDebugInterface");
	if (SUCCEEDED(f(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
	SAFE_RELEASE(&debugController);
#endif
#endif

	IDXGIFactory6* factory = nullptr;
	IDXGIAdapter1* adapter = nullptr;

	CreateDXGIFactory(IID_PPV_ARGS(&factory));
	
	for (UINT adapterIndex = 0;; ++adapterIndex)
	{
		adapter = nullptr;
		if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters1(adapterIndex, &adapter))
		{
			break; // No more adapters
		}
	
		// Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device5), nullptr)))
		{
			break;
		}
	
		SAFE_RELEASE(&adapter);
	}
	
	if (adapter)
	{
		HRESULT hr = S_OK;
		//Create the actual device.
		// TODO: Exception thrown at ___________
		if (SUCCEEDED(hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device5))))
		{
			deviceCreated = true;
		}
	
		SAFE_RELEASE(&adapter);
	}
	else
	{
		//Create warp device if no adapter was found.
		factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter));
		D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device5));
	}
	
	SAFE_RELEASE(&factory);

	return deviceCreated;
}

bool Renderer::CreateCommandQueue()
{
	bool commandQueueCreated = true;

	D3D12_COMMAND_QUEUE_DESC cqd = {};
	HRESULT hr = device5->CreateCommandQueue(&cqd, IID_PPV_ARGS(&this->commandQueue));

	if (hr != S_OK)
	{
		commandQueueCreated = false;
	}

	return commandQueueCreated;
}

void Renderer::CreateAllocatorAndListTemporary()
{
	device5->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));

	device5->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator,
		nullptr,
		IID_PPV_ARGS(&commandList5));

	commandList5->Close();
}

bool Renderer::CreateSwapChain()
{
	bool swapChainCreated = false;

	IDXGIFactory4* factory = nullptr;
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));

	if (hr != S_OK)
	{
		// TODO: Errorbox or no? Göra en klass för debugsträngar?
		OutputDebugStringA("Error: Failed to create DXGIFactory!\n");
		return false;
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
		this->commandQueue,
		*this->window->GetHwnd(),
		&scDesc,
		nullptr,
		nullptr,
		&swapChain1)))
	{
		swapChainCreated = true;
		if (SUCCEEDED(swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain4))))
		{
			swapChain4->Release();
		}
	}

	SAFE_RELEASE(&factory);

	return swapChainCreated;
}

bool Renderer::CreateRootSignature()
{
	this->rootSignature = new RootSignature();

	HRESULT hr = device5->CreateRootSignature(
		0,
		this->rootSignature->GetBlob()->GetBufferPointer(),
		this->rootSignature->GetBlob()->GetBufferSize(),
		IID_PPV_ARGS(this->rootSignature->GetRootSig()));

	if (hr != S_OK)
	{
		OutputDebugStringA("Error: Failed to create RootSignature!\n");
		return false;
	}
	return true;
}

bool Renderer::CreatePSO(RenderTask* renderTask)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = *renderTask->Getgpsd();

	// Set the rootSignature in the pipeline state object descriptor
	renderTask->Getgpsd()->pRootSignature = *this->rootSignature->GetRootSig();

	// Create pipelineStateObject
	HRESULT hr = device5->CreateGraphicsPipelineState(renderTask->Getgpsd(), IID_PPV_ARGS(renderTask->GetPipelineState()->GetPSO()));
	if (!SUCCEEDED(hr))
	{
		return false;
	}

	return true;
}

bool Renderer::CreateRenderTarget(RenderTargetTypes rtt)
{
	if (rtt == RenderTargetTypes::SWAPCHAIN)
	{
		RenderTarget* renderTarget = new RenderTarget();

		// Fill out descriptor for the render target views
		D3D12_DESCRIPTOR_HEAP_DESC dhd = {};
		dhd.NumDescriptors = NUM_SWAP_BUFFERS;
		dhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

		// Create descriptorHeap for the renderTarget
		HRESULT hr = this->device5->CreateDescriptorHeap(&dhd, IID_PPV_ARGS(renderTarget->GetRTHeap()));
		if (hr != S_OK)
		{
			OutputDebugStringA("Error: Failed to create DescriptorHeap For SwapChainRenderTarget!\n");
			return false;
		}

		// Set the size of the descriptor
		renderTarget->SetRTDescriptorSize(this->device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

		D3D12_CPU_DESCRIPTOR_HANDLE cdh = (*renderTarget->GetRTHeap())->GetCPUDescriptorHandleForHeapStart();

		// Connect the renderTargets to the swapchain, so that the swapchain can easily swap between these two renderTargets
		for (UINT i = 0; i < NUM_SWAP_BUFFERS; i++)
		{
			hr = swapChain4->GetBuffer(i, IID_PPV_ARGS(renderTarget->GetRenderTarget(i)));
			if (hr != S_OK)
			{
				OutputDebugStringA("Error: Failed to GetBuffer from RenderTarget to Swapchain!\n");
				return false;
			}

			device5->CreateRenderTargetView(*renderTarget->GetRenderTarget(i), nullptr, cdh);
			cdh.ptr += renderTarget->GetRTDescriptorSize();
		}

		renderTarget->CreateViewport();
		renderTarget->CreateScissorRect();

		this->renderTargetsHolder[rtt].push_back(renderTarget);
		return true;
	}
	else if (rtt == RenderTargetTypes::RENDERTARGET)
	{
		//this->renderTargets.push_back(new DetachedRenderTarget());
	}
	else if (rtt == RenderTargetTypes::DEPTH)
	{
		//this->renderTargets.push_back(new DepthRenderTarget());
	}

	return false;
}

