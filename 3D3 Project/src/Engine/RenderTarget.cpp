#include "RenderTarget.h"

RenderTarget::RenderTarget(
	ID3D12Device5* device,
	unsigned int width, unsigned int height,
	DescriptorHeap* descriptorHeap_RTV)
{
	this->width = width;
	this->height = height;
	
	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 0;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	
	// View Desc
	D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	
	// Create resources and RTVs
	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
	{
		Resource* resource = new Resource(
			device,
			&resourceDesc,
			nullptr,
			L"Render_Target_Resource",
			D3D12_RESOURCE_STATE_GENERIC_READ);
	
		this->resources.push_back(resource);

		unsigned int dhIndex = descriptorHeap_RTV->GetNextDescriptorHeapIndex(1);
		D3D12_CPU_DESCRIPTOR_HANDLE cdh = descriptorHeap_RTV->GetCPUHeapAt(dhIndex);
		device->CreateRenderTargetView(resource->GetID3D12Resource1(), &viewDesc, cdh);
	}
	
	this->CreateViewport();
	this->CreateScissorRect();
}

RenderTarget::RenderTarget(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;

	for (unsigned int i = 0; i < NUM_SWAP_BUFFERS; i++)
	{
		this->resources.push_back(new Resource());
	}

	this->CreateViewport();
	this->CreateScissorRect();
}

RenderTarget::~RenderTarget()
{
	for (Resource* resource : this->resources)
	{
		delete resource;
	}
}

Resource* RenderTarget::GetResource(unsigned int index) const
{
	return this->resources[index];
}

const D3D12_VIEWPORT* RenderTarget::GetViewPort() const
{
	return &this->viewport;
}

const D3D12_RECT* RenderTarget::GetScissorRect() const
{
	return &this->scissorRect;
}

void RenderTarget::CreateViewport()
{
	this->viewport.TopLeftX = 0.0f;
	this->viewport.TopLeftY = 0.0f;
	this->viewport.Width = (float)this->width;
	this->viewport.Height = (float)this->height;
	this->viewport.MinDepth = 0.0f;
	this->viewport.MaxDepth = 1.0f;
}

void RenderTarget::CreateScissorRect()
{
	this->scissorRect.left = (long)0;
	this->scissorRect.right = (long)this->width;
	this->scissorRect.top = (long)0;
	this->scissorRect.bottom = (long)this->height;
}
