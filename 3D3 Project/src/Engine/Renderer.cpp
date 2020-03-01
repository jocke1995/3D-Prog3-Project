#include "Renderer.h"

Renderer::Renderer()
{
	// Nothing here yet
}

Renderer::~Renderer()
{
	CloseHandle(this->eventHandle);
	SAFE_RELEASE(&this->fence);

	SAFE_RELEASE(&this->device5);
	SAFE_RELEASE(&this->swapChain4);

	SAFE_RELEASE(&this->commandQueue);
	// TEMP
	SAFE_RELEASE(&this->commandList5);
	SAFE_RELEASE(&this->commandAllocator);

	delete this->rootSignature;

	for (auto it : constantBuffers)
	{
		delete it.second;
	}

	for (auto rts1 : this->renderTargetsHolder)
		for (auto rts2 : rts1.second)
			delete rts2;
}

void Renderer::InitD3D12(HWND *hwnd)
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
	this->TempCreateFence();

	// Create SwapChain
	if (!this->CreateSwapChain(hwnd))
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

	// Create constantBuffers
	

	ConstantBuffer* transformBuffer = this->CreateConstantBuffer(L"CB_Translate", 1000, CONSTANT_BUFFER_TYPE::CB_PER_OBJECT);
	if (transformBuffer == nullptr)
	{
		OutputDebugStringA("Error: Failed to create TransformBuffer!\n");
	}
	
}

ConstantBuffer* Renderer::CreateConstantBuffer(std::wstring name, unsigned int size, CONSTANT_BUFFER_TYPE type)
{
	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.CreationNodeMask = 1; //used when multi-gpu
	heapProperties.VisibleNodeMask = 1; //used when multi-gpu
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	unsigned int entrySize;
	switch (type)
	{
	case CONSTANT_BUFFER_TYPE::CB_PER_OBJECT:
		entrySize = sizeof(CB_PER_OBJECT); // 16 float
	}

	resourceDesc.Width = size * entrySize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ConstantBuffer* CB = new ConstantBuffer(name, size, entrySize);

	ID3D12Resource1** constantBufferResource = CB->GetResource();

	HRESULT hr = device5->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(constantBufferResource)
	);

	if (FAILED(hr))
		return nullptr;

	constantBuffers[ConstantBufferIndex::CB_TRANSFORM] = CB;

	// TODO: Fix name
	(*constantBufferResource)->SetName(name.c_str());

	return CB;
}

// TODO: Skall vi göra "olika sorters" vertex buffers, sedan skapa dom direkt här? eller ska man få välja parametrar?
void Renderer::CreateVertexBuffer(Mesh* mesh)
{
	// TODO: Use default heap aswell?
	D3D12_HEAP_PROPERTIES hp = {};
	hp.Type = D3D12_HEAP_TYPE_UPLOAD;
	hp.CreationNodeMask = 1;
	hp.VisibleNodeMask = 1;

	size_t size = mesh->GetSize();
	D3D12_RESOURCE_DESC rd = {};
	rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	rd.Width = size; // numVertices * Sizeof(vertices)
	rd.Height = 1;
	rd.DepthOrArraySize = 1;
	rd.MipLevels = 1;
	rd.SampleDesc.Count = 1;
	rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	ID3D12Resource1** resource = mesh->GetVBResource();
	this->device5->CreateCommittedResource(
		&hp,
		D3D12_HEAP_FLAG_NONE,
		&rd,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(resource));

	(*resource)->SetName(L"VB_Mesh");

	mesh->SetData();
}

void Renderer::AddRenderTask(RenderTask* renderTask)
{
	this->CreatePSO(renderTask);
	this->renderTasks.push_back(renderTask);
}

void Renderer::Execute()
{
	int backBufferIndex = this->swapChain4->GetCurrentBackBufferIndex();
	for (auto tasks : this->renderTasks)
	{
		tasks->Execute(this->commandAllocator, this->commandList5, *this->rootSignature->GetRootSig(), backBufferIndex);
	}

	ID3D12CommandList* listsToExecute[] = { this->commandList5 };
	this->commandQueue->ExecuteCommandLists(ARRAYSIZE(listsToExecute), listsToExecute);

	WaitForGPU();

	this->swapChain4->Present(0, 0);
}

RenderTarget* Renderer::GetRenderTarget(RenderTargetTypes rtt, int index)
{
	return this->renderTargetsHolder[rtt].at(index);
}

ConstantBuffer* Renderer::GetConstantBuffer(ConstantBufferIndex index)
{
	return this->constantBuffers[index];
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
	HMODULE mD3D12 = LoadLibrary(L"D3D12.dll"); // ist�llet f�r GetModuleHandle

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

bool Renderer::CreateSwapChain(HWND *hwnd)
{
	bool swapChainCreated = false;

	IDXGIFactory4* factory = nullptr;
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));

	if (hr != S_OK)
	{
		// TODO: Errorbox or no? G�ra en klass f�r debugstr�ngar?
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
		*hwnd,
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
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = *renderTask->GetGpsd();

	// Set the rootSignature in the pipeline state object descriptor
	renderTask->GetGpsd()->pRootSignature = *this->rootSignature->GetRootSig();

	// Create pipelineStateObject
	HRESULT hr = device5->CreateGraphicsPipelineState(renderTask->GetGpsd(), IID_PPV_ARGS(renderTask->GetPipelineState()->GetPSO()));
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

		UINT size = this->device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		renderTarget->CreateDescriptorHeap(DESCRIPTOR_HEAP_TYPES::RTV, size);

		// Create descriptorHeap for the renderTarget
		D3D12_DESCRIPTOR_HEAP_DESC *dhd = renderTarget->GetDescriptorHeap()->GetDesc();
		ID3D12DescriptorHeap** descriptorHeap = renderTarget->GetDescriptorHeap()->GetID3D12DescriptorHeap();
		HRESULT hr = this->device5->CreateDescriptorHeap(dhd, IID_PPV_ARGS(descriptorHeap));
		if (hr != S_OK)
		{
			OutputDebugStringA("Error: Failed to create DescriptorHeap For SwapChainRenderTarget!\n");
			return false;
		}

		renderTarget->GetDescriptorHeap()->SetCPUGPUHeapStart();

		D3D12_CPU_DESCRIPTOR_HANDLE cdh = *renderTarget->GetDescriptorHeap()->GetCPUHeapAt(0);

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
			cdh.ptr += renderTarget->GetDescriptorHeap()->GetHandleIncrementSize();
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

void Renderer::TempCreateFence()
{
	device5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	fenceValue = 1;

	// Event handle to use for GPU synchronization
	eventHandle = CreateEvent(0, false, false, 0);
}

void Renderer::WaitForGPU()
{
	const UINT64 oldFence = fenceValue;
	commandQueue->Signal(fence, oldFence);
	fenceValue++;

	//Wait until command queue is done.
	if (fence->GetCompletedValue() < oldFence)
	{
		fence->SetEventOnCompletion(oldFence, eventHandle);
		WaitForSingleObject(eventHandle, INFINITE);
	}
}


	// TODO: Ska vi g�ra detta p� alla "objekt"? Vad g�r det f�r skillnad?
	// ID3D12PipelineState* pso = (*renderTask->GetPipelineState()->GetPSO());
	// pso->SetName(L"PSO?");