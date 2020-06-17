#include "DepthBuffer.h"

DepthBuffer::DepthBuffer(ID3D12Device5* device, unsigned int width, unsigned int height)
{
	this->descriptorHeap = new DescriptorHeap(device, DESCRIPTOR_HEAP_TYPE::DSV);
	this->width = width;
	this->height = height;

	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.CreationNodeMask = 1; //used when multi-gpu
	heapProperties.VisibleNodeMask = 1; //used when multi-gpu
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 0;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.DepthStencil.Depth = 1;
	clearValue.Format = DXGI_FORMAT_D32_FLOAT;

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&this->resource)
	);

	if (FAILED(hr))
	{
		OutputDebugStringA("Error: Failed to create DepthBuffer!\n");
	}

	D3D12_CPU_DESCRIPTOR_HANDLE cdh = this->descriptorHeap->GetCPUHeapAt(0);
	device->CreateDepthStencilView(this->resource, &depthStencilDesc, cdh);

	this->CreateViewport(width, height);
	this->CreateScissorRect(width, height);
}

DepthBuffer::~DepthBuffer()
{
	SAFE_RELEASE(&this->resource);
	delete this->descriptorHeap;
}

DescriptorHeap* DepthBuffer::GetDescriptorHeap() const
{
	return this->descriptorHeap;
}

const D3D12_VIEWPORT* DepthBuffer::GetViewPort() const
{
	return &this->viewport;
}

const D3D12_RECT* DepthBuffer::GetScissorRect() const
{
	return &this->scissorRect;
}

void DepthBuffer::CreateViewport(unsigned int width, unsigned int height)
{
	this->viewport.TopLeftX = 0.0f;
	this->viewport.TopLeftY = 0.0f;
	this->viewport.Width = (float)width;
	this->viewport.Height = (float)height;
	this->viewport.MinDepth = 0.0f;
	this->viewport.MaxDepth = 1.0f;
}

void DepthBuffer::CreateScissorRect(unsigned int width, unsigned int height)
{
	this->scissorRect.left = (long)0;
	this->scissorRect.right = (long)width;
	this->scissorRect.top = (long)0;
	this->scissorRect.bottom = (long)height;
}
