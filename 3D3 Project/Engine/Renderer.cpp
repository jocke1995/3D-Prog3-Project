#include "Renderer.h"

Renderer::Renderer()
{
	// Nothing here yet
}

Renderer::~Renderer()
{
	delete this->window;
	delete this->rootSignature;
}

void Renderer::CreateWindow(HINSTANCE hInstance, int nCmdShow, int screenWidth, int screenHeight, bool fullScreen, LPCTSTR windowName, LPCTSTR windowTitle)
{
	this->window = new Window(hInstance, nCmdShow, screenWidth, screenHeight, fullScreen, windowName, windowTitle);
}

void Renderer::InitD3D12()
{
	// Init DX stuff... device, swapchain etc..

	// Create Device
	if (!this->CreateDevice())
	{
		// TODO: Errorbox or no? Göra en klass för debugsträngar?
		OutputDebugStringA("Error: Failed to create Device!");
	}

	// Create CommandQueue
	if (!this->CreateCommandQueue())
	{
		// TODO: Errorbox or no? Göra en klass för debugsträngar?
		OutputDebugStringA("Error: Failed to create CommandQueue!");
	}

	// TEMP
	this->CreateAllocatorAndListTemporary();

	// Create SwapChain
	if (!this->CreateSwapChain())
	{
		// TODO: Errorbox or no? Göra en klass för debugsträngar?
		OutputDebugStringA("Error: Failed to create SwapChain!");
	}

	// Create Rootsignature
	this->rootSignature = new RootSignature(this->device5);
}

void Renderer::Execute()
{
	while (!this->window->ExitWindow())
	{
		// Här inne ska vi fylla commandolistorna senare
	}
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
		OutputDebugStringA("Error: Failed to create DXGIFactory!");
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
