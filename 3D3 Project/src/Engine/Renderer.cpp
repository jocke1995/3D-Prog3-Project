#include "Renderer.h"

Renderer::Renderer()
{
	this->renderTasks.resize(RENDER_TASK_TYPE::NR_OF_RENDERTASKS);
	this->copyTasks.resize(COPY_TASK_TYPE::NR_OF_COPYTASKS);
	this->computeTasks.resize(COMPUTE_TASK_TYPE::NR_OF_COMPUTETASKS);
}

Renderer::~Renderer()
{
	this->WaitForFrame();
	this->threadpool->WaitForThreads(THREAD_FLAG::ALL);
	this->threadpool->ExitThreads();

	CloseHandle(this->eventHandle);
	SAFE_RELEASE(&this->fenceFrame);
	
	SAFE_RELEASE(&this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]);
	SAFE_RELEASE(&this->commandQueues[COMMAND_INTERFACE_TYPE::COMPUTE_TYPE]);
	SAFE_RELEASE(&this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]);

	delete this->rootSignature;

	delete this->swapChain;
	
	delete this->depthBuffer;

	delete this->descriptorHeap_CBV_UAV_SRV;

	for (auto computeTask : this->computeTasks)
		delete computeTask;

	for (auto copyTask : this->copyTasks)
		delete copyTask;

	for (auto renderTask : this->renderTasks)
		delete renderTask;

	// Resources -------------
	delete this->copySourceResource;
	delete this->copyDestResource;
	// -----------------------

	SAFE_RELEASE(&this->device5);

	delete this->camera;

	delete this->threadpool;
}

void Renderer::InitD3D12(const HWND *hwnd, HINSTANCE hInstance)
{
	// Camera
	this->camera = new Camera(L"default_cam", hInstance, *hwnd);

	// Create Device
	if (!this->CreateDevice())
	{
		Log::PrintError(Log::ErrorType::ENGINE, "Failed to Create Device\n");
	}

	// Create CommandQueues (direct and copy)
	this->CreateCommandQueues();

	// Fence for WaitForFrame();
	this->CreateFences();

	// Create SwapChain
	this->CreateSwapChain(hwnd);

	// ThreadPool
	int numCPUs = std::thread::hardware_concurrency();
	if (numCPUs == 0) numCPUs = 1; // function not supported ej vettig dator
	this->threadpool = new ThreadPool(numCPUs); // Set num threads to number of cores of the cpu

	// Create Main DepthBuffer
	this->CreateDepthBuffer();
	
	// Create Rootsignature
	this->CreateRootSignature();

	// Create resource for the copy queue (a float4 vector with color)
	this->copySourceResource = new Resource(this->device5, sizeof(float4), RESOURCE_TYPE::UPLOAD,  L"copySourceResource");
	this->copyDestResource = new Resource(this->device5, sizeof(float4), RESOURCE_TYPE::RESOURCE_COPY, L"copyDestResource");

	// Create DescriptorHeap
	this->InitDescriptorHeap();

	AssetLoader::Get()->SetDevice(this->device5);

	this->InitRenderTasks();
}

std::vector<Mesh*>* Renderer::LoadModel(std::wstring path)
{
	bool loadedBefore = false;
	std::vector<Mesh*>* meshes = AssetLoader::Get()->LoadModel(path, &loadedBefore);

	// Only Create the SRVs if its the first time the model is loaded
	if (!loadedBefore)
	{
		for (Mesh* mesh : *meshes)
		{
			this->CreateShaderResourceView(mesh);
		}
	}
	return meshes;
}

void Renderer::SetSceneToDraw(Scene* scene)
{
	this->entitiesToDraw.clear();

	std::map<std::string, Entity*> entities = *scene->GetEntities();
	for (auto const& [entityName, entity] : entities)
	{
		// Only add the entities that actually should be drawn
		component::RenderComponent* rc = entity->GetComponent<component::RenderComponent>();
		if (rc != nullptr)
		{
			this->entitiesToDraw.push_back(entity);
		}
	}

	// Update renderTasks with new entities and mainCamera
	// TODO: slå ihop till en loop..? problem: duplicering av kod
	this->SetRenderTasksEntities();
	this->SetMainCamera(scene->GetMainCamera());

	this->scene = scene;
}

bool Renderer::AddEntityToDraw(Entity* entity)
{
	component::RenderComponent* rc = entity->GetComponent<component::RenderComponent>();
	if (rc != nullptr)
	{
		this->entitiesToDraw.push_back(entity);
		this->SetRenderTasksEntities();
		return true;
	}
	return false;	
}

bool Renderer::RemoveEntityToDraw(Entity* entity)
{
	component::RenderComponent* rc = entity->GetComponent<component::RenderComponent>();
	if (rc != nullptr)
	{
		for (int i = 0; i < this->entitiesToDraw.size(); i++)
		{
			if (*entity == this->entitiesToDraw[i])
			{
				this->entitiesToDraw.erase(this->entitiesToDraw.begin() + i);
				this->SetRenderTasksEntities();
				return true;
			}
		}
	}
	return false;
}

int Compare(const void* a, const void* b)
{
	if (*(double*)a > * (double*)b)
		return 1;
	else if (*(double*)a < *(double*)b)
		return -1;
	else
		return 0;
}

void Renderer::UpdateScene(double dt)
{
	this->scene->UpdateScene(dt);
}

void Renderer::SortEntitiesByDistance()
{
	struct DistEnt
	{
		double distance;
		Entity* entity;
	};

	int nrOfEntities = this->entitiesToDraw.size();

	DistEnt* distEntArr = new DistEnt[nrOfEntities];

	// Get all the distances of each objects and store them by ID and distance
	XMFLOAT3 camPos = this->camera->GetPosition();
	for (int i = 0; i < nrOfEntities; i++)
	{
		XMFLOAT3 objectPos = this->entitiesToDraw.at(i)->GetComponent<component::RenderComponent>()->GetTransform()->GetPosition();

		double distance = sqrt(	pow(camPos.x - objectPos.x, 2) +
								pow(camPos.y - objectPos.y, 2) +
								pow(camPos.z - objectPos.z, 2));

		// Save the object alongside its distance to the camera
		distEntArr[i].distance = distance;
		distEntArr[i].entity = this->entitiesToDraw.at(i);
	}

	// InsertionSort (because its best case is O(N)), 
	// and since this is sorted ((((((EVERY FRAME)))))) this is a good choice of sorting algorithm
	int j = 0;
	DistEnt distEntTemp = {};
	for (int i = 1; i < nrOfEntities; i++)
	{
		j = i;
		while (j > 0 && (distEntArr[j - 1].distance > distEntArr[j].distance))
		{
			// Swap
			distEntTemp = distEntArr[j - 1];
			distEntArr[j - 1] = distEntArr[j];
			distEntArr[j] = distEntTemp;
			j--;
		}
	}

	// Fill the vector with sorted array
	this->entitiesToDraw.clear();
	for (int i = 0; i < nrOfEntities; i++)
	{
		this->entitiesToDraw.push_back(distEntArr[i].entity);
	}

	// Free memory
	delete distEntArr;

	// Update the entity-arrays inside the rendertasks
	this->SetRenderTasksEntities();
}

void Renderer::Execute()
{
	IDXGISwapChain4* dx12SwapChain = ((SwapChain*)this->swapChain)->GetDX12SwapChain();
	int backBufferIndex = dx12SwapChain->GetCurrentBackBufferIndex();
	int commandInterfaceIndex = this->frameCounter++ % 2;
	
	// Fill queue with copytasks and execute them in parallell
	for (CopyTask* copyTask : this->copyTasks)
	{
		copyTask->SetCommandInterfaceIndex(commandInterfaceIndex);
		this->threadpool->AddTask(copyTask, THREAD_FLAG::COPY);
	}

	// Fill queue with computeTasks and execute them in parallell
	for (ComputeTask* computeTask : this->computeTasks)
	{
		computeTask->SetCommandInterfaceIndex(commandInterfaceIndex);
		this->threadpool->AddTask(computeTask, THREAD_FLAG::COMPUTE);
	}

	// Fill queue with rendertasks and execute them in parallell
	for (RenderTask* renderTask : this->renderTasks)
	{
		renderTask->SetBackBufferIndex(backBufferIndex);
		renderTask->SetCommandInterfaceIndex(commandInterfaceIndex);
		this->threadpool->AddTask(renderTask, THREAD_FLAG::DIRECT);
	}

	/* COPY QUEUE --------------------------------------------------------------- */
	// Wait for CopyTasks to complete
	this->threadpool->WaitForThreads(THREAD_FLAG::COPY);

	// Execute copy tasks
	this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]->ExecuteCommandLists(
		this->copyCommandLists[commandInterfaceIndex].size(),
		this->copyCommandLists[commandInterfaceIndex].data());

	UINT64 copyFenceValue = this->fenceFrameValue;
	this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]->Signal(this->fenceFrame, copyFenceValue + 1);
	this->fenceFrameValue++;
	/* --------------------------------------------------------------- */

	/* COMPUTE QUEUE --------------------------------------------------------------- */
	// Wait for ComputeTasks to complete
	this->threadpool->WaitForThreads(THREAD_FLAG::COMPUTE);

	// Wait for copyTask to finish
	this->commandQueues[COMMAND_INTERFACE_TYPE::COMPUTE_TYPE]->Wait(this->fenceFrame, copyFenceValue + 1);

	// Execute Compute tasks
	this->commandQueues[COMMAND_INTERFACE_TYPE::COMPUTE_TYPE]->ExecuteCommandLists(
		this->computeCommandLists[commandInterfaceIndex].size(),
		this->computeCommandLists[commandInterfaceIndex].data());

	int computeFenceValue = this->fenceFrameValue;
	this->commandQueues[COMMAND_INTERFACE_TYPE::COMPUTE_TYPE]->Signal(this->fenceFrame, computeFenceValue + 1);
	this->fenceFrameValue++;

	/* --------------------------------------------------------------- */

	/* RENDER QUEUE --------------------------------------------------------------- */
	// Wait for DirectTasks to complete
	this->threadpool->WaitForThreads(THREAD_FLAG::DIRECT | THREAD_FLAG::ALL);

	// Wait for ComputeTask to finish
	this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]->Wait(this->fenceFrame, computeFenceValue + 1);

	this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]->ExecuteCommandLists(
		this->directCommandLists[commandInterfaceIndex].size(), 
		this->directCommandLists[commandInterfaceIndex].data());

	/* --------------------------------------------------------------- */

	// Wait if the CPU is to far ahead of the gpu
	WaitForFrame();

	dx12SwapChain->Present(0, 0);
}

ThreadPool* Renderer::GetThreadPool() const
{
	return this->threadpool;
}

Camera* Renderer::GetCamera() const
{
	return this->camera;
}

void Renderer::SetMainCamera(Camera* camera)
{
	for (auto renderTask : this->renderTasks)
		renderTask->SetCamera(camera);
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
		if (SUCCEEDED(hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device5))))
		{
			deviceCreated = true;
		}
		else
		{
			Log::PrintError(Log::ErrorType::ENGINE, "Failed to create Device\n");
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

void Renderer::CreateCommandQueues()
{
	// Direct
	D3D12_COMMAND_QUEUE_DESC cqdDirect = {};
	cqdDirect.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hr;
	hr = device5->CreateCommandQueue(&cqdDirect, IID_PPV_ARGS(&this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]));
	if (FAILED(hr))
	{
		Log::PrintError(Log::ErrorType::ENGINE, "Failed to Create Direct CommandQueue\n");
	}

	// Compute
	D3D12_COMMAND_QUEUE_DESC cqdCompute = {};
	cqdCompute.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	hr = device5->CreateCommandQueue(&cqdCompute, IID_PPV_ARGS(&this->commandQueues[COMMAND_INTERFACE_TYPE::COMPUTE_TYPE]));
	if (FAILED(hr))
	{
		Log::PrintError(Log::ErrorType::ENGINE, "Failed to Create Compute CommandQueue\n");
	}

	// Copy
	D3D12_COMMAND_QUEUE_DESC cqdCopy = {};
	cqdCopy.Type = D3D12_COMMAND_LIST_TYPE_COPY;
	hr = device5->CreateCommandQueue(&cqdCopy, IID_PPV_ARGS(&this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]));
	if (FAILED(hr))
	{
		Log::PrintError(Log::ErrorType::ENGINE, "Failed to Create Copy CommandQueue\n");
	}
}

void Renderer::CreateSwapChain(const HWND *hwnd)
{
	this->swapChain = new SwapChain(device5, hwnd, this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]);
}

void Renderer::CreateDepthBuffer()
{
	this->depthBuffer = new DepthBuffer(this->device5, 800, 600);
	//this->depthBuffer = new DepthBuffer(this->device5, 1920, 1080);
}

void Renderer::CreateRootSignature()
{
	this->rootSignature = new RootSignature(this->device5);
}


void Renderer::InitRenderTasks()
{
#pragma region ForwardRendering
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsdForwardRender = {};
	gpsdForwardRender.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// RenderTarget
	gpsdForwardRender.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsdForwardRender.NumRenderTargets = 1;
	// Depthstencil usage
	gpsdForwardRender.SampleDesc.Count = 1;
	gpsdForwardRender.SampleMask = UINT_MAX;
	// Rasterizer behaviour
	gpsdForwardRender.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpsdForwardRender.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	gpsdForwardRender.RasterizerState.FrontCounterClockwise = false;

	// Specify Blend descriptions
	D3D12_RENDER_TARGET_BLEND_DESC defaultRTdesc = {
		false, false,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP, D3D12_COLOR_WRITE_ENABLE_ALL };
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		gpsdForwardRender.BlendState.RenderTarget[i] = defaultRTdesc;

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

	gpsdForwardRender.DepthStencilState = dsd;
	gpsdForwardRender.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*> gpsdForwardRenderVector;
	gpsdForwardRenderVector.push_back(&gpsdForwardRender);

	RenderTask* forwardRenderTask = new FowardRenderTask(this->device5, 
		this->rootSignature, 
		L"VertexShader.hlsl", L"PixelShader.hlsl", 
		&gpsdForwardRenderVector, 
		L"ForwardRenderingPSO",
		COMMAND_INTERFACE_TYPE::DIRECT_TYPE);

	forwardRenderTask->AddRenderTarget(this->swapChain);
	forwardRenderTask->SetDepthBuffer(this->depthBuffer);
	forwardRenderTask->SetDescriptorHeap_CBV_UAV_SRV(this->descriptorHeap_CBV_UAV_SRV);

	// Resources ------------
	forwardRenderTask->AddResource(this->copyDestResource);
#pragma endregion ForwardRendering
#pragma region Blend
	// ------------------------ TASK 2: BLEND ---------------------------- FRONTCULL

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
	dsdBlend.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
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

	// ------------------------ TASK 2: BLEND ---------------------------- BACKCULL

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
	dsdBlend.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	gpsdBlendBackCull.DepthStencilState = dsdBlend;
	gpsdBlendBackCull.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	gpsdBlendVector.push_back(&gpsdBlendFrontCull);
	gpsdBlendVector.push_back(&gpsdBlendBackCull);

	RenderTask* blendRenderTask = new BlendRenderTask(this->device5,
		this->rootSignature,
		L"BlendVertex.hlsl",
		L"BlendPixel.hlsl",
		&gpsdBlendVector,
		L"BlendPSO",
		COMMAND_INTERFACE_TYPE::DIRECT_TYPE);

	blendRenderTask->AddRenderTarget(this->swapChain);
	blendRenderTask->SetDepthBuffer(this->depthBuffer);
	blendRenderTask->SetDescriptorHeap_CBV_UAV_SRV(this->descriptorHeap_CBV_UAV_SRV);

#pragma endregion Blend
	// :-----------------------------TASK CopyColor:-----------------------------
	CopyTask* copyTask = new CopyColorTask(this->device5, COMMAND_INTERFACE_TYPE::COPY_TYPE);

	copyTask->AddResource(this->copySourceResource);
	copyTask->AddResource(this->copyDestResource);

	// :-----------------------------TASK ComputeTest:-----------------------------
	ComputeTask* computeTestTask = new ComputeTestTask(this->device5,
		this->rootSignature,
		L"ComputeTest.hlsl",
		L"ComputeTestPSO",
		COMMAND_INTERFACE_TYPE::COMPUTE_TYPE);

	computeTestTask->AddResource(this->copyDestResource);

	// Add the tasks to desired vectors so they can be used in renderer
	/* -------------------------------------------------------------- */


	/* ------------------------- CopyQueue Tasks ------------------------ */
	this->copyTasks[COPY_TASK_TYPE::COPY_COLOR] = copyTask;

	// Pushback in the order of execution
	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
		this->copyCommandLists[i].push_back(copyTask->GetCommandList(i));

	/* ------------------------- ComputeQueue Tasks ------------------------ */
	this->computeTasks[COMPUTE_TASK_TYPE::COMPUTE_TEST] = computeTestTask;

	// Pushback in the order of execution
	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
		this->computeCommandLists[i].push_back(computeTestTask->GetCommandList(i));

	/* ------------------------- DirectQueue Tasks ---------------------- */
	this->renderTasks[RENDER_TASK_TYPE::FORWARD_RENDER] = forwardRenderTask;
	this->renderTasks[RENDER_TASK_TYPE::BLEND] = blendRenderTask;

	// Pushback in the order of execution
	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
		this->directCommandLists[i].push_back(forwardRenderTask->GetCommandList(i));

	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
		this->directCommandLists[i].push_back(blendRenderTask->GetCommandList(i));
}

void Renderer::SetRenderTasksEntities()
{
	for (RenderTask* rendertask : this->renderTasks)
	{
		rendertask->SetEntitiesToDraw(&this->entitiesToDraw);
	}
}

void Renderer::InitDescriptorHeap()
{
	this->descriptorHeap_CBV_UAV_SRV = new DescriptorHeap(this->device5, DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV);
}

void Renderer::CreateShaderResourceView(Mesh* mesh)
{
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = this->descriptorHeap_CBV_UAV_SRV->GetCPUHeapAt(mesh->GetVertexDataIndex());

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
	HRESULT hr = device5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->fenceFrame));

	if (FAILED(hr))
	{
		Log::PrintError(Log::ErrorType::ENGINE, "Faile to Create Fence\n");
	}
	this->fenceFrameValue = 1;

	// Event handle to use for GPU synchronization
	this->eventHandle = CreateEvent(0, false, false, 0);
}

void Renderer::WaitForFrame()
{
	const UINT64 oldFenceValue = this->fenceFrameValue;
	const UINT64 newFenceValue = oldFenceValue + 1;
	this->fenceFrameValue++;

	this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]->Signal(this->fenceFrame, newFenceValue);

	// Wait for direct to finish before we can copy again 
	this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]->Wait(this->fenceFrame, newFenceValue);

	//Wait until command queue is done.
	int nrOfFenceChanges = 3;
	int fenceValuesToBeAhead = (NUM_SWAP_BUFFERS - 1) * nrOfFenceChanges;
	if (this->fenceFrame->GetCompletedValue() < newFenceValue - fenceValuesToBeAhead)
	{
		this->fenceFrame->SetEventOnCompletion(newFenceValue - fenceValuesToBeAhead, this->eventHandle);
		WaitForSingleObject(this->eventHandle, INFINITE);
	}
}
