#include "Renderer.h"

D3D12::D3D12Timer timer;

Renderer::Renderer()
{
	// Nothing here yet
}

Renderer::~Renderer()
{
	CloseHandle(this->eventHandle);
	SAFE_RELEASE(&this->fenceFrame);

	SAFE_RELEASE(&this->device5);
	
	SAFE_RELEASE(&this->commandQueue);

	delete this->rootSignature;

	delete this->swapChain;
	delete this->depthBuffer;

	delete this->descriptorHeap;

	for (auto renderTask : this->renderTasks)
		delete renderTask;
}

void Renderer::InitD3D12(HWND *hwnd)
{
	// Create Device
	if (!this->CreateDevice())
	{
		OutputDebugStringA("Error: Failed to create Device!\n");
	}

	// Timer
	timer.init(this->device5, RenderTaskType::NR_OF_RENDERTASKS);

	// Create CommandQueue
	if (!this->CreateCommandQueue())
	{
		OutputDebugStringA("Error: Failed to create CommandQueue!\n");
	}

	// TEMP
	this->CreateFences();

	// Create SwapChain
	if (!this->CreateSwapChain(hwnd))
	{
		OutputDebugStringA("Error: Failed to create SwapChain!\n");
	}

	// Create Main DepthBuffer
	if (!this->CreateDepthBuffer())
	{
		OutputDebugStringA("Error: Failed to create DepthBuffer!\n");
	}
	
	// Create Rootsignature
	if (!this->CreateRootSignature())
	{
		OutputDebugStringA("Error: Failed to create RootSignature!\n");
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
	gpsdTest.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

	// Specify Blend descriptions
	D3D12_RENDER_TARGET_BLEND_DESC defaultRTdesc = {
		false, false,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP, D3D12_COLOR_WRITE_ENABLE_ALL };
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		gpsdTest.BlendState.RenderTarget[i] = defaultRTdesc;

	// Depth descriptor
	D3D12_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// Om pixels depth är lägre än den gamla så ritas den nya ut

	// DepthStencil
	dsd.StencilEnable = false;
	dsd.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	dsd.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	const D3D12_DEPTH_STENCILOP_DESC defaultStencilOP{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
	dsd.FrontFace = defaultStencilOP;
	dsd.BackFace = defaultStencilOP;

	gpsdTest.DepthStencilState = dsd;
	gpsdTest.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*> gpsdTestVector;
	gpsdTestVector.push_back(&gpsdTest);
	
	RenderTask* testTask = new RenderTaskTest(this->device5, this->rootSignature, L"VertexShader.hlsl", L"PixelShader.hlsl", &gpsdTestVector);
	testTask->AddRenderTarget(this->swapChain);
	testTask->SetDepthBuffer(this->depthBuffer);
	testTask->SetDescriptorHeap(this->descriptorHeap);
	
	this->renderTasks.push_back(testTask);
	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
		this->listsToExecute[i].push_back(testTask->GetCommandList(i));

	// :-----------------------------TASK 2:----------------------------- BLEND

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsdBlendFrontCull = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsdBlendBackCull = {};
	std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*> gpsdBlendVector;

	gpsdBlendFrontCull.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// RenderTarget
	gpsdBlendFrontCull.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsdBlendFrontCull.NumRenderTargets = 1;
	// Depthstencil usage
	gpsdBlendFrontCull.SampleDesc.Count = 1;
	gpsdBlendFrontCull.SampleMask = UINT_MAX;
	// Rasterizer behaviour
	gpsdBlendFrontCull.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpsdBlendFrontCull.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;

	// Specify Blend descriptions
	D3D12_RENDER_TARGET_BLEND_DESC blendRTdesc{};
	blendRTdesc.BlendEnable = true;
	blendRTdesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendRTdesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendRTdesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendRTdesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendRTdesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blendRTdesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendRTdesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	

	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		gpsdBlendFrontCull.BlendState.RenderTarget[i] = blendRTdesc;


	// Depth descriptor
	D3D12_DEPTH_STENCIL_DESC dsdBlend = {};
	dsdBlend.DepthEnable = true;
	dsdBlend.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	dsdBlend.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// Om pixels depth är lägre än den gamla så ritas den nya ut

	// DepthStencil
	dsdBlend.StencilEnable = false;
	dsdBlend.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	dsdBlend.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	const D3D12_DEPTH_STENCILOP_DESC blendStencilOP{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
	dsdBlend.FrontFace = blendStencilOP;
	dsdBlend.BackFace = blendStencilOP;

	gpsdBlendFrontCull.DepthStencilState = dsdBlend;
	gpsdBlendFrontCull.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ------------------------ TEST 2 BACKCULL ----------------------------

	gpsdBlendBackCull.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// RenderTarget
	gpsdBlendBackCull.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsdBlendBackCull.NumRenderTargets = 1;
	// Depthstencil usage
	gpsdBlendBackCull.SampleDesc.Count = 1;
	gpsdBlendBackCull.SampleMask = UINT_MAX;
	// Rasterizer behaviour
	gpsdBlendBackCull.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpsdBlendBackCull.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		gpsdBlendBackCull.BlendState.RenderTarget[i] = blendRTdesc;

	// DepthStencil
	dsdBlend.StencilEnable = false;

	gpsdBlendBackCull.DepthStencilState = dsdBlend;
	gpsdBlendBackCull.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	
	gpsdBlendVector.push_back(&gpsdBlendFrontCull);
	gpsdBlendVector.push_back(&gpsdBlendBackCull);

	RenderTask* blendTask = new RenderTaskBlend(this->device5, this->rootSignature, L"BlendVertex.hlsl", L"BlendPixel.hlsl", &gpsdBlendVector);
	blendTask->AddRenderTarget(this->swapChain);
	blendTask->SetDepthBuffer(this->depthBuffer);
	blendTask->SetDescriptorHeap(this->descriptorHeap);

	this->renderTasks.push_back(blendTask);
	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
		this->listsToExecute[i].push_back(blendTask->GetCommandList(i));
}

Mesh* Renderer::CreateMesh(std::wstring path)
{
	// TODO: Är detta en dålig lösning?
	bool loadedBefore = false;
	Mesh* mesh = AssetLoader::Get().LoadMesh(path, &loadedBefore);

	// Only Create the SRV if its the first time the mesh is loaded
	if(!loadedBefore)
		this->CreateShaderResourceView(mesh);

	return mesh;
}

// Add the object to each rendertask desired for the object.
void Renderer::AddObjectToTasks(Object* object)
{
	DrawOptions* drawOptions = object->GetDrawOptions();

	if (drawOptions->test == true)
		this->renderTasks[RenderTaskType::TEST]->AddObject(object);

	if (drawOptions->blend == true)
		this->renderTasks[RenderTaskType::BLEND]->AddObject(object);
}

void Renderer::SetCamera(Camera* camera)
{
	for (auto renderTask : this->renderTasks)
		renderTask->SetCamera(camera);
}

void Renderer::Execute()
{
	IDXGISwapChain4* dx12SwapChain = ((SwapChain*)this->swapChain)->GetDX12SwapChain();
	int backBufferIndex = dx12SwapChain->GetCurrentBackBufferIndex();

	for (auto task : this->renderTasks)
		task->Execute(this->rootSignature->GetRootSig(), backBufferIndex);

	// Wait for CPU

	UINT64 queueFreq;
	this->commandQueue->GetTimestampFrequency(&queueFreq);

	this->commandQueue->ExecuteCommandLists(
		this->listsToExecute[backBufferIndex].size(), 
		this->listsToExecute[backBufferIndex].data()
	);
	
	// Wait for GPU
	WaitForFrame();

	dx12SwapChain->Present(0, 0);

	//get time in ms
	double timestampToMs = (1.0 / queueFreq) * 1000.0;

	// 0 = TEST, 1 = BLEND
	D3D12::GPUTimestampPair drawTimeTest = timer.getTimestampPair(0);
	D3D12::GPUTimestampPair drawTimeBlend = timer.getTimestampPair(1);

	UINT64 dt = drawTimeTest.Stop - drawTimeTest.Start;
	double timeInMs = dt * timestampToMs;
	
	char buf[100];
	sprintf_s(buf, "GPU TEST TASK  : %fms\n", timeInMs);
	OutputDebugStringA(buf);

	dt = drawTimeBlend.Stop - drawTimeBlend.Start;
	timeInMs = dt * timestampToMs;

	sprintf_s(buf, "GPU BLEND TASK : %fms\n\n", timeInMs);
	OutputDebugStringA(buf);
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

bool Renderer::CreateSwapChain(HWND *hwnd)
{
	// TODO: Detta
	swapChain = new SwapChain(device5, hwnd, this->commandQueue);

	return true;
}

bool Renderer::CreateDepthBuffer()
{
	this->depthBuffer = new DepthBuffer(this->device5, 800, 600);
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

void Renderer::CreateFences()
{
	device5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->fenceFrame));

	this->fenceFrameValue = 1;

	// Event handle to use for GPU synchronization
	this->eventHandle = CreateEvent(0, false, false, 0);
}

void Renderer::WaitForFrame()
{
	const UINT64 oldFence = this->fenceFrameValue;
	commandQueue->Signal(this->fenceFrame, oldFence);
	this->fenceFrameValue++;

	//Wait until command queue is done.
	if (this->fenceFrame->GetCompletedValue() < oldFence - 1)
	{
		this->fenceFrame->SetEventOnCompletion(oldFence, this->eventHandle);
		WaitForSingleObject(this->eventHandle, INFINITE);
	}
}


	// TODO: Ska vi g�ra detta p� alla "objekt"? Vad g�r det f�r skillnad?
	// ID3D12PipelineState* pso = (*renderTask->GetPipelineState()->GetPSO());
	// pso->SetName(L"PSO?");