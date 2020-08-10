#include "ShadowInfo.h"

ShadowInfo::ShadowInfo(
	unsigned int textureWidth, unsigned int textureHeight,
	ID3D12Device5* device,
	DescriptorHeap* dh_DSV,
	DescriptorHeap* dh_SRV)
{
	this->CreateResource(device, textureWidth, textureHeight);

	this->CreateDSV(device, dh_DSV);
	this->CreateSRV(device, dh_SRV);

	this->renderView = new RenderView(textureWidth, textureHeight);
}

ShadowInfo::~ShadowInfo()
{
	delete this->resource;

	delete this->DSV;
	delete this->SRV;

	delete this->renderView;
}

Resource* ShadowInfo::GetResource() const
{
	return this->resource;
}

DepthStencilView* ShadowInfo::GetDSV() const
{
	return this->DSV;
}

ShaderResourceView* ShadowInfo::GetSRV() const
{
	return this->SRV;
}

RenderView* ShadowInfo::GetRenderView() const
{
	return this->renderView;
}

void ShadowInfo::CreateResource(ID3D12Device5* device, unsigned int width, unsigned int height)
{
	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	desc.Width = width;
	desc.Height = height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Alignment = 0;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	this->resource = new Resource(
		device,
		&desc,
		&clearValue,
		L"LIGHTDEPTH_DEFAULT_RESOURCE",
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void ShadowInfo::CreateDSV(ID3D12Device5* device, DescriptorHeap* dh_DSV)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = D3D12_DSV_FLAG_NONE;
	dsvd.Texture2D.MipSlice = 0;

	this->DSV = new DepthStencilView(
		device,
		dh_DSV,
		this->resource,
		&dsvd);
}

void ShadowInfo::CreateSRV(ID3D12Device5* device, DescriptorHeap* dh_SRV)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvd.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.ResourceMinLODClamp = 0.0f;
	srvd.Texture2D.PlaneSlice = 0;

	this->SRV = new ShaderResourceView(
		device,
		dh_SRV,
		&srvd,
		this->resource);
}