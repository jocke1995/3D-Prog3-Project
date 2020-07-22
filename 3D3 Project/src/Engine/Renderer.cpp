#include "Renderer.h"

Renderer::Renderer()
{
	this->renderTasks.resize(RENDER_TASK_TYPE::NR_OF_RENDERTASKS);
	this->copyTasks.resize(COPY_TASK_TYPE::NR_OF_COPYTASKS);
	this->computeTasks.resize(COMPUTE_TASK_TYPE::NR_OF_COMPUTETASKS);
}

Renderer::~Renderer()
{
	Log::Print("----------------------------  Renderer Destructor  ----------------------------------\n");
	this->WaitForFrame();
	Log::Print("1\n");
	this->threadpool->WaitForThreads(THREAD_FLAG::ALL);
	Log::Print("2\n");
	this->threadpool->ExitThreads();
	Log::Print("3\n");
	SAFE_RELEASE(&this->fenceFrame);
	Log::Print("4\n");
	if (!CloseHandle(this->eventHandle))
	{
		Log::Print("Failed To Close Handle... ErrorCode: %d\n", GetLastError());
	}
	Log::Print("5\n");
	SAFE_RELEASE(&this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]);
	Log::Print("6\n");
	SAFE_RELEASE(&this->commandQueues[COMMAND_INTERFACE_TYPE::COMPUTE_TYPE]);
	Log::Print("7\n");
	SAFE_RELEASE(&this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]);
	Log::Print("8\n");
	delete this->rootSignature;
	Log::Print("9\n");
	delete this->swapChain;
	Log::Print("10\n");
	delete this->depthBuffer;
	Log::Print("11\n");
	delete this->descriptorHeap_CBV_UAV_SRV;
	Log::Print("12\n");

	for (auto computeTask : this->computeTasks)
		delete computeTask;

	for (auto copyTask : this->copyTasks)
		delete copyTask;

	for (auto renderTask : this->renderTasks)
		delete renderTask;

	SAFE_RELEASE(&this->device5);
	Log::Print("15\n");
	delete this->camera;
	Log::Print("16\n");
	delete this->threadpool;
	Log::Print("17\n");

	// temp
	delete this->tempCommandInterface;
}

void Renderer::InitD3D12(const HWND *hwnd, HINSTANCE hInstance)
{
	// Camera
	this->camera = new Camera(L"default_cam", hInstance, *hwnd);

	// Create Device
	if (!this->CreateDevice())
	{
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to Create Device\n");
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

	// Create DescriptorHeap
	this->InitDescriptorHeap();

	// Init assetloader by giving it a pointer to the device
	AssetLoader::Get(this->device5);

	this->InitRenderTasks();
	
	// temp
	this->tempCommandInterface = new CommandInterface(this->device5, COMMAND_INTERFACE_TYPE::DIRECT_TYPE);
}

std::vector<Mesh*>* Renderer::LoadModel(std::wstring path)
{
	bool loadedBefore = false;
	std::vector<Mesh*>* meshes = AssetLoader::Get()->LoadModel(path, &loadedBefore);

	// Only Create the SRVs if its the first time the model is loaded
	if (!loadedBefore)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		desc.Buffer.FirstElement = 0;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		for (Mesh* mesh : *meshes)
		{
			desc.Buffer.NumElements = mesh->GetNumVertices();
			desc.Buffer.StructureByteStride = sizeof(Mesh::Vertex);

			this->CreateShaderResourceView(	mesh->GetDescriptorHeapIndex(),
											&desc,
											mesh->GetResourceVertices());

			// This function wont make a SRV if the texture already has one bound to it. (Safe to use)
			this->CreateSRVForTexture(mesh->GetTexture(TEXTURE_TYPE::AMBIENT));
			this->CreateSRVForTexture(mesh->GetTexture(TEXTURE_TYPE::DIFFUSE));
			this->CreateSRVForTexture(mesh->GetTexture(TEXTURE_TYPE::SPECULAR));
			this->CreateSRVForTexture(mesh->GetTexture(TEXTURE_TYPE::NORMAL));
			this->CreateSRVForTexture(mesh->GetTexture(TEXTURE_TYPE::EMISSIVE));
		}
	}
	return meshes;
}

Texture* Renderer::LoadTexture(std::wstring path)
{
	Texture* texture = AssetLoader::Get()->LoadTexture(path);

	if (texture == nullptr)
	{
		return nullptr;
	}

	if (this->CreateSRVForTexture(texture) == false)
	{
		return nullptr;
	}

	return texture;
}

void Renderer::SetSceneToDraw(Scene* scene)
{
	this->renderComponents.clear();

	std::map<std::string, Entity*> entities = *scene->GetEntities();
	for (auto const& [entityName, entity] : entities)
	{
		// Only add the entities that actually should be drawn
		component::MeshComponent* mc = entity->GetComponent<component::MeshComponent>();
		if (mc != nullptr)
		{
			component::TransformComponent* tc = entity->GetComponent<component::TransformComponent>();
			if (tc != nullptr)
			{
				this->renderComponents.push_back(std::make_pair(mc, tc));

				// Send the Textures to GPU here later, so that textures aren't in memory if they aren't used
			}
		}

		component::DirectionalLightComponent* dlc = entity->GetComponent<component::DirectionalLightComponent>();
		if (dlc != nullptr)
		{
			this->dirLightComponents.push_back(dlc);
		}
	}

	this->SetRenderTasksRenderComponents();
	this->SetRenderTasksMainCamera(scene->GetMainCamera());

	this->currActiveScene = scene;
}

//bool Renderer::AddEntityToDraw(Entity* entity)
//{
//	component::RenderComponent* rc = entity->GetComponent<component::RenderComponent>();
//	if (rc != nullptr)
//	{
//		this->renderComponents.push_back(entity);
//		this->SetRenderTasksRenderComponents();
//		return true;
//	}
//	return false;	
//}
//
//bool Renderer::RemoveEntityToDraw(Entity* entity)
//{
//	component::RenderComponent* rc = entity->GetComponent<component::RenderComponent>();
//	if (rc != nullptr)
//	{
//		for (int i = 0; i < this->renderComponents.size(); i++)
//		{
//			if (*entity == this->renderComponents[i])
//			{
//				this->renderComponents.erase(this->renderComponents.begin() + i);
//				this->SetRenderTasksRenderComponents();
//				return true;
//			}
//		}
//	}
//	return false;
//}

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
	this->currActiveScene->UpdateScene(dt);
}

void Renderer::SortEntitiesByDistance()
{
	struct DistFromCamera
	{
		double distance;
		component::MeshComponent* mc;
		component::TransformComponent* tc;
	};

	int nrOfRenderComponents = this->renderComponents.size();

	DistFromCamera* distFromCamArr = new DistFromCamera[nrOfRenderComponents];

	// Get all the distances of each objects and store them by ID and distance
	XMFLOAT3 camPos = this->camera->GetPosition();
	for (int i = 0; i < nrOfRenderComponents; i++)
	{
		XMFLOAT3 objectPos = this->renderComponents.at(i).second->GetTransform()->GetPositionXMFLOAT3();

		double distance = sqrt(	pow(camPos.x - objectPos.x, 2) +
								pow(camPos.y - objectPos.y, 2) +
								pow(camPos.z - objectPos.z, 2));

		// Save the object alongside its distance to the camera
		distFromCamArr[i].distance = distance;
		distFromCamArr[i].mc = this->renderComponents.at(i).first;
		distFromCamArr[i].tc = this->renderComponents.at(i).second;
	}

	// InsertionSort (because its best case is O(N)), 
	// and since this is sorted ((((((EVERY FRAME)))))) this is a good choice of sorting algorithm
	int j = 0;
	DistFromCamera distFromCamArrTemp = {};
	for (int i = 1; i < nrOfRenderComponents; i++)
	{
		j = i;
		while (j > 0 && (distFromCamArr[j - 1].distance > distFromCamArr[j].distance))
		{
			// Swap
			distFromCamArrTemp = distFromCamArr[j - 1];
			distFromCamArr[j - 1] = distFromCamArr[j];
			distFromCamArr[j] = distFromCamArrTemp;
			j--;
		}
	}

	// Fill the vector with sorted array
	this->renderComponents.clear();
	for (int i = 0; i < nrOfRenderComponents; i++)
	{
		this->renderComponents.push_back(std::make_pair(distFromCamArr[i].mc, distFromCamArr[i].tc));
	}

	// Free memory
	delete distFromCamArr;

	// Update the entity-arrays inside the rendertasks
	this->SetRenderTasksRenderComponents();
}

void Renderer::Execute()
{
	IDXGISwapChain4* dx12SwapChain = ((SwapChain*)this->swapChain)->GetDX12SwapChain();
	int backBufferIndex = dx12SwapChain->GetCurrentBackBufferIndex();
	int commandInterfaceIndex = this->frameCounter++ % 2;
	
	// Fill queue with rendertasks and execute them in parallell
	for (RenderTask* renderTask : this->renderTasks)
	{
		renderTask->SetBackBufferIndex(backBufferIndex);
		renderTask->SetCommandInterfaceIndex(commandInterfaceIndex);
		this->threadpool->AddTask(renderTask, THREAD_FLAG::DIRECT);
		//renderTask->Execute();	// Non-multithreaded version of this
	}

	/* RENDER QUEUE --------------------------------------------------------------- */
	// Wait for the threads which records the c-lists to complete
	this->threadpool->WaitForThreads(THREAD_FLAG::DIRECT | THREAD_FLAG::ALL);

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

ID3D12Device5* Renderer::GetDevice() const
{
	return this->device5;
}

void Renderer::SetRenderTasksMainCamera(Camera* camera)
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
			Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to create Device\n");
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
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to Create Direct CommandQueue\n");
	}
	this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]->SetName(L"DirectQueue");

	// Compute
	D3D12_COMMAND_QUEUE_DESC cqdCompute = {};
	cqdCompute.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	hr = device5->CreateCommandQueue(&cqdCompute, IID_PPV_ARGS(&this->commandQueues[COMMAND_INTERFACE_TYPE::COMPUTE_TYPE]));
	if (FAILED(hr))
	{
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to Create Compute CommandQueue\n");
	}
	this->commandQueues[COMMAND_INTERFACE_TYPE::COMPUTE_TYPE]->SetName(L"ComputeQueue");

	// Copy
	D3D12_COMMAND_QUEUE_DESC cqdCopy = {};
	cqdCopy.Type = D3D12_COMMAND_LIST_TYPE_COPY;
	hr = device5->CreateCommandQueue(&cqdCopy, IID_PPV_ARGS(&this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]));
	if (FAILED(hr))
	{
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to Create Copy CommandQueue\n");
	}
	this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]->SetName(L"CopyQueue");
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

	// Add the tasks to desired vectors so they can be used in renderer
	/* -------------------------------------------------------------- */


	/* ------------------------- CopyQueue Tasks ------------------------ */

	// None atm

	/* ------------------------- ComputeQueue Tasks ------------------------ */

	// None atm

	/* ------------------------- DirectQueue Tasks ---------------------- */
	this->renderTasks[RENDER_TASK_TYPE::FORWARD_RENDER] = forwardRenderTask;
	this->renderTasks[RENDER_TASK_TYPE::BLEND] = blendRenderTask;

	// Pushback in the order of execution
	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
		this->directCommandLists[i].push_back(forwardRenderTask->GetCommandList(i));

	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
		this->directCommandLists[i].push_back(blendRenderTask->GetCommandList(i));
}

void Renderer::SetRenderTasksRenderComponents()
{
	for (RenderTask* rendertask : this->renderTasks)
	{
		rendertask->SetRenderComponents(&this->renderComponents);
	}
}

void Renderer::SetRenderTasksDirLightComponents()
{
}

void Renderer::InitDescriptorHeap()
{
	this->descriptorHeap_CBV_UAV_SRV = new DescriptorHeap(this->device5, DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV);
}

void Renderer::CreateShaderResourceView(unsigned int descriptorHeapIndex,
	D3D12_SHADER_RESOURCE_VIEW_DESC* desc,
	Resource* resource)
{
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = this->descriptorHeap_CBV_UAV_SRV->GetCPUHeapAt(descriptorHeapIndex);

	this->device5->CreateShaderResourceView(resource->GetID3D12Resource1(), desc, cdh);
}

bool Renderer::CreateSRVForTexture(Texture* texture)
{
	// Check if the texture already has a SRV attached to it
	if (texture->IsBoundToSRV() == false)
	{
		// Tell the texture that it now has a srv binded to it
		texture->Bind();

		// Upload texturedata
		texture->UploadTextureData(this->device5,
			this->tempCommandInterface,
			this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]);
		WaitForGpu();

		D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		desc.Format = *texture->GetGDXIFormat();
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		this->CreateShaderResourceView(texture->GetDescriptorHeapIndex(), &desc, texture->GetResource());

		return true;
	}
	
	return false;
}

void Renderer::CreateConstantBufferView(unsigned int descriptorHeapIndex, unsigned int size, Resource* resource)
{
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = this->descriptorHeap_CBV_UAV_SRV->GetCPUHeapAt(descriptorHeapIndex);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvd = {};
	cbvd.BufferLocation = resource->GetGPUVirtualAdress();
	cbvd.SizeInBytes = size;
	this->device5->CreateConstantBufferView(&cbvd, cdh);
}

void Renderer::CreateFences()
{
	HRESULT hr = device5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->fenceFrame));

	if (FAILED(hr))
	{
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to Create Fence\n");
	}
	this->fenceFrameValue = 1;

	// Event handle to use for GPU synchronization
	this->eventHandle = CreateEvent(0, false, false, 0);
}

void Renderer::WaitForFrame()
{
	const UINT64 oldFenceValue = this->fenceFrameValue; // 1
	const UINT64 newFenceValue = oldFenceValue + 1; // 2
	this->fenceFrameValue++;

	this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]->Signal(this->fenceFrame, newFenceValue);

	//Wait until command queue is done.
	int nrOfFenceChanges = 1;
	int fenceValuesToBeAhead = (NUM_SWAP_BUFFERS - 1) * nrOfFenceChanges;
	if (this->fenceFrame->GetCompletedValue() < newFenceValue - fenceValuesToBeAhead)
	{
		this->fenceFrame->SetEventOnCompletion(newFenceValue - fenceValuesToBeAhead, this->eventHandle);
		WaitForSingleObject(this->eventHandle, INFINITE);
	}
}

void Renderer::WaitForGpu()
{
	//Signal and increment the fence value.
	const UINT64 oldFenceValue = this->fenceFrameValue;
	this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]->Signal(this->fenceFrame, oldFenceValue);
	this->fenceFrameValue++;

	//Wait until command queue is done.
	if (this->fenceFrame->GetCompletedValue() < oldFenceValue)
	{
		this->fenceFrame->SetEventOnCompletion(oldFenceValue, eventHandle);
		WaitForSingleObject(eventHandle, INFINITE);
	}
}
