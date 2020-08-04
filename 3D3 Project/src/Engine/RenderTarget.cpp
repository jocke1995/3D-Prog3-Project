#include "RenderTarget.h"

RenderTarget::RenderTarget(ID3D12Device5* device, unsigned int width, unsigned int height, unsigned int nrOf)
{
	//this->descriptorHeap = new DescriptorHeap(device, DESCRIPTOR_HEAP_TYPE::RTV);
	//this->resources.resize(nrOf);
	//this->width = width;
	//this->height = height;
	//
	//D3D12_HEAP_PROPERTIES heapProperties = {};
	//heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//heapProperties.CreationNodeMask = 1; //used when multi-gpu
	//heapProperties.VisibleNodeMask = 1; //used when multi-gpu
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	//
	//D3D12_RESOURCE_DESC resourceDesc = {};
	//resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//resourceDesc.Width = width;
	//resourceDesc.Height = height;
	//resourceDesc.DepthOrArraySize = 1;
	//resourceDesc.MipLevels = 0;
	//resourceDesc.SampleDesc.Count = 1;
	//resourceDesc.SampleDesc.Quality = 0;
	//resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	//resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	//
	//// View Desc
	//D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
	//viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//
	//// Create resource
	//for (int i = 0; i < nrOf; i++)
	//{
	//	device->CreateCommittedResource(
	//		&heapProperties,
	//		D3D12_HEAP_FLAG_NONE,
	//		&resourceDesc,
	//		D3D12_RESOURCE_STATE_GENERIC_READ,
	//		nullptr,
	//		IID_PPV_ARGS(&this->resources[i])
	//	);
	//
	//	D3D12_CPU_DESCRIPTOR_HANDLE cdh = this->descriptorHeap->GetCPUHeapAt(i);
	//	device->CreateRenderTargetView(this->resources[i], &viewDesc, cdh);
	//}
	//
	//this->CreateViewport(width, height);
	//this->CreateScissorRect(width, height);
}

RenderTarget::RenderTarget()
{

}

RenderTarget::~RenderTarget()
{
	for (int i = 0; i < this->resources.size(); i++)
	{
		SAFE_RELEASE(&this->resources[i]);
	}
}

ID3D12Resource1* RenderTarget::GetResource(UINT index) const
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


void RenderTarget::CreateViewport(unsigned int width, unsigned int height)
{
	this->viewport.TopLeftX = 0.0f;
	this->viewport.TopLeftY = 0.0f;
	this->viewport.Width = (float)width;
	this->viewport.Height = (float)height;
	this->viewport.MinDepth = 0.0f;
	this->viewport.MaxDepth = 1.0f;
}

void RenderTarget::CreateScissorRect(unsigned int width, unsigned int height)
{
	this->scissorRect.left = (long)0;
	this->scissorRect.right = (long)width;
	this->scissorRect.top = (long)0;
	this->scissorRect.bottom = (long)height;
}
