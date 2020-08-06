#include "DepthStencilView.h"

DepthStencilView::DepthStencilView(
	ID3D12Device5* device,
	unsigned int width, unsigned int height,
	DescriptorHeap* descriptorHeap_DSV)
{
	this->CreateResource(device, width, height);
	this->CreateDSV(device, descriptorHeap_DSV);
}

DepthStencilView::~DepthStencilView()
{
	delete this->resource;
}

void DepthStencilView::CreateResource(ID3D12Device5* device, unsigned int width, unsigned int height)
{
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

	this->resource = new Resource(
		device,
		&resourceDesc,
		&clearValue,
		L"DSV_DEFAULT_RESOURCE",
		D3D12_RESOURCE_STATE_DEPTH_WRITE);
}

void DepthStencilView::CreateDSV(ID3D12Device5* device, DescriptorHeap* descriptorHeap_DSV)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CPU_DESCRIPTOR_HANDLE cdh = descriptorHeap_DSV->GetCPUHeapAt(0);
	device->CreateDepthStencilView(this->resource->GetID3D12Resource1(), &depthStencilDesc, cdh);
}

