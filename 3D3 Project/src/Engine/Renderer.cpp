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
	this->WaitForFrame(0);
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
	for (auto& pair : this->descriptorHeaps)
	{
		delete pair.second;
	}

	for (auto computeTask : this->computeTasks)
		delete computeTask;

	for (auto copyTask : this->copyTasks)
		delete copyTask;

	for (auto renderTask : this->renderTasks)
		delete renderTask;

	SAFE_RELEASE(&this->device5);
	Log::Print("13\n");
	delete this->camera;
	Log::Print("14\n");
	delete this->threadpool;
	Log::Print("15\n");

	delete this->lightCBPool;
	Log::Print("16\n");
	delete this->cbPerScene;
	Log::Print("17\n");

	delete this->cbPerFrameData;
	delete this->cbPerFrame;
	Log::Print("18\n");

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

	// Create DescriptorHeap
	this->InitDescriptorHeaps();

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

	// Init assetloader by giving it a pointer to the device
	AssetLoader::Get(this->device5, this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV]);
	
	// Pool to handle constantBuffers for the lights
	this->lightCBPool = new LightCBVPool(this->device5, this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV]);

	// Allocate memory for cbPerScene
	unsigned int CB_PER_SCENE_SizeAligned = (sizeof(CB_PER_SCENE_STRUCT) + 255) & ~255;
	this->cbPerScene = new ConstantBufferView(
		this->device5, 
		CB_PER_SCENE_SizeAligned,
		L"CB_PER_SCENE_DEFAULT",
		this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV]->GetNextDescriptorHeapIndex(1),
		this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV]
		);

	// Allocate memory for cbPerFrame
	unsigned int CB_PER_Frame_SizeAligned = (sizeof(CB_PER_FRAME_STRUCT) + 255) & ~255;
	this->cbPerFrame = new ConstantBufferView(
		this->device5,
		CB_PER_Frame_SizeAligned,
		L"CB_PER_FRAME_DEFAULT",
		this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV]->GetNextDescriptorHeapIndex(1),
		this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV]
	);

	this->cbPerFrameData = new CB_PER_FRAME_STRUCT();
	this->cbPerFrameData->camPos = this->camera->GetPositionFloat3();

	this->InitRenderTasks();

	// temp, used to transmit textures/meshes to the default memory on GPU when loaded
	this->tempCommandInterface = new CommandInterface(this->device5, COMMAND_INTERFACE_TYPE::DIRECT_TYPE);
}

std::vector<Mesh*>* Renderer::LoadModel(std::wstring path)
{
	bool loadedBefore = false;
	std::vector<Mesh*>* meshes = AssetLoader::Get()->LoadModel(path, &loadedBefore);

	// ------------------------------ TEMPORARY CODE ------------------------------ 
	// Only Upload to default heaps if its the first time its loaded
	if (!loadedBefore)
	{
		for (Mesh* mesh : *meshes)
		{
			// Upload to Default heap
			mesh->UploadToDefault(
				this->device5,
				this->tempCommandInterface,
				this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]);
			this->WaitForGpu();

			// Wont upload data if its already up.. TEMPORARY safecheck inside the texture class
			for (unsigned int i = 0; i < TEXTURE_TYPE::NUM_TEXTURE_TYPES; i++)
			{
				TEXTURE_TYPE type = static_cast<TEXTURE_TYPE>(i);
				Texture* texture = mesh->GetTexture(type);
				texture->UploadTextureData(
					this->device5,
					this->tempCommandInterface,
					this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]);
				this->WaitForGpu();
			}
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

	// ------------------------------ TEMPORARY CODE ------------------------------ 
	// Wont upload data if its already up.. TEMPORARY safecheck inside the texture class
	texture->UploadTextureData(
		this->device5,
		this->tempCommandInterface,
		this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]);
	this->WaitForGpu();

	return texture;
}

// Handle the components thats used for rendering
void Renderer::SetSceneToDraw(Scene* scene)
{
	// Reset
	this->renderComponents.clear();
	this->directionalLights.clear();
	this->pointLights.clear();
	this->spotLights.clear();
	this->lightCBPool->FreeConstantBuffers();
	this->copyTasks[COPY_TASK_TYPE::COPY_PER_FRAME]->Clear();

	// Handle and structure the components in the scene
#pragma region HandleComponents
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
				// or save index to a queue and then submit all textures together later..
			}
		}

		component::DirectionalLightComponent* dlc = entity->GetComponent<component::DirectionalLightComponent>();
		if (dlc != nullptr)
		{
			// Assign resource from resourcePool
			ConstantBufferView* cbd = this->lightCBPool->GetFreeConstantBufferDefault(LIGHT_TYPE::DIRECTIONAL_LIGHT);

			// Save in renderer
			this->directionalLights.push_back(std::make_pair(dlc, cbd));
		}

		component::PointLightComponent* plc = entity->GetComponent<component::PointLightComponent>();
		if (plc != nullptr)
		{
			// Assign resource from resourcePool
			ConstantBufferView* cbd = this->lightCBPool->GetFreeConstantBufferDefault(LIGHT_TYPE::POINT_LIGHT);

			// Save in renderer
			this->pointLights.push_back(std::make_pair(plc, cbd));
		}

		component::SpotLightComponent* slc = entity->GetComponent<component::SpotLightComponent>();
		if (slc != nullptr)
		{
			// Assign resource from resourcePool
			ConstantBufferView* cbd = this->lightCBPool->GetFreeConstantBufferDefault(LIGHT_TYPE::SPOT_LIGHT);

			// Save in renderer
			this->spotLights.push_back(std::make_pair(slc, cbd));
		}
	}
#pragma endregion HandleComponents
	
	// Setup Per-scene data and send to GPU
#pragma region Prepare_CbPerScene
	CB_PER_SCENE_STRUCT cps = {};
		// ----- directional lights -----
		cps.Num_Dir_Lights = this->directionalLights.size();
		unsigned int index = 0;
		for (auto& pair : this->directionalLights)
		{
			cps.dirLightIndices[index].x = pair.second->GetDescriptorHeapIndex();
			index++;
		}
		// ----- directional lights -----

		// ----- point lights -----
		cps.Num_Point_Lights = this->pointLights.size();
		index = 0;
		for (auto& pair : this->pointLights)
		{
			cps.pointLightIndices[index].x = pair.second->GetDescriptorHeapIndex();
			index++;
		}
		// ----- point lights -----

		// ----- spot lights -----
		cps.Num_Spot_Lights = this->spotLights.size();
		index = 0;
		for (auto& pair : this->spotLights)
		{
			cps.spotLightIndices[index].x = pair.second->GetDescriptorHeapIndex();
			index++;
		}
		// ----- spot lights -----

	// Upload CB_PER_SCENE to defaultheap
	this->TempCopyResource( 
		this->cbPerScene->GetUploadResource(),
		this->cbPerScene->GetDefaultResource(),
		&cps);
#pragma endregion Prepare_CbPerScene

	// Add Per-frame data to the copy queue
#pragma region Prepare_CbPerFrame
	for (auto& pair : this->directionalLights)
	{
		void* data = pair.first->GetDirectionalLight();
		ConstantBufferView* cbd = pair.second;
		this->copyTasks[COPY_TASK_TYPE::COPY_PER_FRAME]->AddDataToUpdate(&std::make_pair(data, cbd));
	}
	for (auto& pair : this->pointLights)
	{
		void* data = pair.first->GetPointLight();
		ConstantBufferView* cbd = pair.second;
		this->copyTasks[COPY_TASK_TYPE::COPY_PER_FRAME]->AddDataToUpdate(&std::make_pair(data, cbd));
	}
	for (auto& pair : this->spotLights)
	{
		void* data = pair.first->GetSpotLight();
		ConstantBufferView* cbd = pair.second;
		this->copyTasks[COPY_TASK_TYPE::COPY_PER_FRAME]->AddDataToUpdate(&std::make_pair(data, cbd));
	}

	// CB_PER_FRAME_STRUCT
	void* perFrameData = this->cbPerFrameData;
	this->copyTasks[COPY_TASK_TYPE::COPY_PER_FRAME]->AddDataToUpdate(&std::make_pair(perFrameData, this->cbPerFrame));
#pragma endregion Prepare_CbPerFrame

	this->SetRenderTasksRenderComponents();
	this->SetRenderTasksMainCamera(scene->GetMainCamera());

	this->currActiveScene = scene;
}

void Renderer::Update(double dt)
{
	// Update CB_PER_FRAME data
	this->cbPerFrameData->camPos = this->camera->GetPositionFloat3();

	// Update scene
	this->currActiveScene->UpdateScene(dt);
}

void Renderer::SortObjectsByDistance()
{
	struct DistFromCamera
	{
		double distance;
		component::MeshComponent* mc;
		component::TransformComponent* tc;
	};

	int numRenderComponents = this->renderComponents.size();

	DistFromCamera* distFromCamArr = new DistFromCamera[numRenderComponents];

	// Get all the distances of each objects and store them by ID and distance
	XMFLOAT3 camPos = this->camera->GetPosition();
	for (int i = 0; i < numRenderComponents; i++)
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
	for (int i = 1; i < numRenderComponents; i++)
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
	for (int i = 0; i < numRenderComponents; i++)
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
	IDXGISwapChain4* dx12SwapChain = this->swapChain->GetDX12SwapChain();
	int backBufferIndex = dx12SwapChain->GetCurrentBackBufferIndex();
	int commandInterfaceIndex = this->frameCounter++ % 2;

	// Currently there is only 1 copy task.. but later there will be more
	for (CopyTask* copytask : this->copyTasks)
	{
		copytask->SetCommandInterfaceIndex(commandInterfaceIndex);
		//this->threadpool->AddTask(copytask, THREAD_FLAG::COPY_DATA);
		copytask->Execute();	// Non-multithreaded version
	}

	//this->threadpool->WaitForThreads(THREAD_FLAG::COPY_DATA);

	this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]->ExecuteCommandLists(
		this->copyCommandLists[commandInterfaceIndex].size(),
		this->copyCommandLists[commandInterfaceIndex].data());

	UINT64 copyFenceValue = ++this->fenceFrameValue;
	this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]->Signal(this->fenceFrame, copyFenceValue);

	// Fill queue with rendertasks and execute them in parallell
	for (RenderTask* renderTask : this->renderTasks)
	{
		renderTask->SetBackBufferIndex(backBufferIndex);
		renderTask->SetCommandInterfaceIndex(commandInterfaceIndex);
		this->threadpool->AddTask(renderTask, THREAD_FLAG::RENDER);
		//renderTask->Execute();
	}

	/* RENDER QUEUE --------------------------------------------------------------- */
	// Wait for the threads which records the commandlists to complete
	this->threadpool->WaitForThreads(THREAD_FLAG::RENDER | THREAD_FLAG::ALL);
	this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]->Wait(this->fenceFrame, copyFenceValue);


	this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]->ExecuteCommandLists(
		this->directCommandLists[commandInterfaceIndex].size(), 
		this->directCommandLists[commandInterfaceIndex].data());

	/* --------------------------------------------------------------- */

	// Wait if the CPU is to far ahead of the gpu
	this->fenceFrameValue++;

	this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]->Signal(this->fenceFrame, this->fenceFrameValue);
	this->commandQueues[COMMAND_INTERFACE_TYPE::COPY_TYPE]->Wait(this->fenceFrame, this->fenceFrameValue);
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
	
	for (unsigned int adapterIndex = 0;; ++adapterIndex)
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
	RECT rect;
	unsigned int width = 0;
	unsigned int height = 0;
	if (GetWindowRect(*hwnd, &rect))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}

	this->swapChain = new SwapChain(
		device5,
		hwnd,
		width, height,
		this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE],
		this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::RTV]);
}

void Renderer::CreateDepthBuffer()
{
	this->depthBuffer = new DepthBuffer(
		this->device5,
		800, 600,	// width, height
		this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::DSV]);
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
	for (unsigned int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
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
		L"ForwardRenderingPSO");

	forwardRenderTask->SetSwapChain(this->swapChain);
	forwardRenderTask->SetDescriptorHeaps(this->descriptorHeaps);
	forwardRenderTask->AddResource("cbPerFrame", this->cbPerFrame->GetDefaultResource());
	forwardRenderTask->AddResource("cbPerScene", this->cbPerScene->GetDefaultResource());

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


	for (unsigned int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
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

	for (unsigned int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
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
		L"BlendPSO");

	blendRenderTask->SetSwapChain(this->swapChain);
	blendRenderTask->SetDescriptorHeaps(this->descriptorHeaps);
	blendRenderTask->AddResource("cbPerFrame", this->cbPerFrame->GetDefaultResource());
	blendRenderTask->AddResource("cbPerScene", this->cbPerScene->GetDefaultResource());

#pragma endregion Blend

#pragma region CopyPerFrameTask
	CopyTask* copyPerFrameTask = new CopyPerFrameTask(this->device5);
	
#pragma endregion CopyPerFrameTask
	// Add the tasks to desired vectors so they can be used in renderer
	/* -------------------------------------------------------------- */


	/* ------------------------- CopyQueue Tasks ------------------------ */

	this->copyTasks[COPY_TASK_TYPE::COPY_PER_FRAME] = copyPerFrameTask;

	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
		this->copyCommandLists[i].push_back(copyPerFrameTask->GetCommandList(i));

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

void Renderer::InitDescriptorHeaps()
{
	this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV] = new DescriptorHeap(this->device5, DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV);
	this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::RTV] = new DescriptorHeap(this->device5, DESCRIPTOR_HEAP_TYPE::RTV);
	this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::DSV] = new DescriptorHeap(this->device5, DESCRIPTOR_HEAP_TYPE::DSV);
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

void Renderer::WaitForFrame(unsigned int framesToBeAhead)
{
	static constexpr unsigned int nrOfFenceChangesPerFrame = 2;
	unsigned int fenceValuesToBeAhead = framesToBeAhead * nrOfFenceChangesPerFrame;

	//Wait if the CPU is "framesToBeAhead" number of frames ahead of the GPU
	if (this->fenceFrame->GetCompletedValue() < this->fenceFrameValue - fenceValuesToBeAhead)
	{
		this->fenceFrame->SetEventOnCompletion(this->fenceFrameValue - fenceValuesToBeAhead, this->eventHandle);
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

void Renderer::TempCopyResource(Resource* uploadResource, Resource* defaultResource, void* data)
{
	this->tempCommandInterface->Reset(0);
	// Set the data into the upload heap
	uploadResource->SetData(data);

	this->tempCommandInterface->GetCommandList(0)->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		defaultResource->GetID3D12Resource1(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_COPY_DEST));

	// To Defaultheap from Uploadheap
	this->tempCommandInterface->GetCommandList(0)->CopyResource(
		defaultResource->GetID3D12Resource1(),	// Receiever
		uploadResource->GetID3D12Resource1());	// Sender

	this->tempCommandInterface->GetCommandList(0)->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		defaultResource->GetID3D12Resource1(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_COMMON));

	this->tempCommandInterface->GetCommandList(0)->Close();
	ID3D12CommandList* ppCommandLists[] = { this->tempCommandInterface->GetCommandList(0) };
	this->commandQueues[COMMAND_INTERFACE_TYPE::DIRECT_TYPE]->ExecuteCommandLists(ARRAYSIZE(ppCommandLists), ppCommandLists);
	this->WaitForGpu();
}
