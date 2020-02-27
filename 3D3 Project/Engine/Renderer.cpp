#include "Renderer.h"

Renderer::Renderer()
{
	// Nothing here yet
}

Renderer::~Renderer()
{
	for (ConstantBuffer* CB : constantBuffers)
	{
		delete CB;
	}

	delete this->window;
}

void Renderer::CreateWindow(HINSTANCE hInstance, int nCmdShow, int screenWidth, int screenHeight, bool fullScreen, LPCTSTR windowName, LPCTSTR windowTitle)
{
	this->window = new Window(hInstance, nCmdShow, screenWidth, screenHeight, fullScreen, windowName, windowTitle);
}

void Renderer::InitD3D12()
{
	// Init DX stuff... device, swapchain etc..
	if (!this->CreateDevice())
	{
		// TODO: Errorbox or no? Göra en klass för debugsträngar?
		OutputDebugStringA("Not good");
	}
}

ConstantBuffer* Renderer::CreateConstantBuffer(std::wstring name, D3D12_HEAP_TYPE heapType, unsigned int size, CONSTANT_BUFFER_TYPE type)
{
	// TODO: Skapar heap properties efter vi har skapat heapen???????????????????
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
	this->constantBuffers.push_back(CB);

	ID3D12Resource1* constantBufferResource = CB->GetResource();

	device5->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constantBufferResource)
	);

	// TODO: Fix name
	constantBufferResource->SetName(name.c_str());

	return CB;
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
