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
	

	SAFE_RELEASE(&this->commandQueue);
	// TEMP
	SAFE_RELEASE(&this->commandList5);
	SAFE_RELEASE(&this->commandAllocator);

	delete this->rootSignature;

	delete swapChain;

	delete this->descriptorHeap;

	for (auto renderTask : this->renderTasks)
		delete renderTask.second;
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

	// Create DescriptorHeap
	this->InitDescriptorHeap();

	AssetLoader::Get().SetDevice(this->device5);
}

void Renderer::InitRenderTasks()
{
	// :-----------------------------TASK TEST:-----------------------------
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsdTest = {};
	gpsdTest.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// RenderTarget
	gpsdTest.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsdTest.NumRenderTargets = 1;
	// Depthstencil usage
	gpsdTest.SampleDesc.Count = 1;
	gpsdTest.SampleMask = UINT_MAX;
	// Rasterizer behaviour
	gpsdTest.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpsdTest.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	// Specify Blend descriptions
	D3D12_RENDER_TARGET_BLEND_DESC defaultRTdesc = {
		false, false,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP, D3D12_COLOR_WRITE_ENABLE_ALL };
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		gpsdTest.BlendState.RenderTarget[i] = defaultRTdesc;


	RenderTask* testTask = new RenderTaskTest(this->device5, this->rootSignature, L"VertexShader.hlsl", L"PixelShader.hlsl", &gpsdTest);
	testTask->AddRenderTarget(this->swapChain);
	testTask->SetDescriptorHeap(this->descriptorHeap);

	this->renderTasks[RenderTaskType::TEST] = testTask;

	// :-----------------------------TASK 2:-----------------------------
}

// TODO: Skall vi göra "olika sorters" vertex buffers, sedan skapa dom direkt här? eller ska man få välja parametrar?
void Renderer::CreateShaderResourceView(Mesh* mesh)
{
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = this->descriptorHeap->GetCPUHeapAt(mesh->GetVertexDataIndex());

	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};

	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = mesh->GetNumVertices();
	desc.Buffer.StructureByteStride = sizeof(Mesh::Vertex);
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	this->device5->CreateShaderResourceView(mesh->GetResource()->GetID3D12Resource1(), &desc, cdh);
}

Mesh* Renderer::CreateMesh(std::wstring path)
{
	Mesh* mesh = AssetLoader::Get().LoadMesh(path);

	this->CreateShaderResourceView(mesh);

	return mesh;
}

void Renderer::SetObjectsToDraw(RenderTaskType type, std::vector<Object*> *objects)
{
	this->renderTasks[type]->SetObjectsToDraw(objects);
}

void Renderer::SetCamera(RenderTaskType type, Camera* camera)
{
	this->renderTasks[type]->SetCamera(camera);
}

void Renderer::Execute()
{
	// TODO: STEFAN
	IDXGISwapChain4* dx12SwapChain = ((SwapChain*)this->swapChain)->GetDX12SwapChain();
	int backBufferIndex = dx12SwapChain->GetCurrentBackBufferIndex();
	for (auto tasks : this->renderTasks)
	{
		tasks.second->Execute(this->commandAllocator, this->commandList5, this->rootSignature->GetRootSig(), backBufferIndex);
	}

	ID3D12CommandList* listsToExecute[] = { this->commandList5 };
	this->commandQueue->ExecuteCommandLists(ARRAYSIZE(listsToExecute), listsToExecute);

	WaitForGPU();

	dx12SwapChain->Present(0, 0);
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
	// TODO: Detta
	swapChain = new SwapChain(device5, hwnd, this->commandQueue);

	return true;
}

bool Renderer::CreateRootSignature()
{
	this->rootSignature = new RootSignature(this->device5);

	
	return true;
}

void Renderer::InitDescriptorHeap()
{
	this->descriptorHeap = new DescriptorHeap(this->device5, DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV);
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