#include "RenderTarget.h"

RenderTarget::RenderTarget()
{
}

RenderTarget::~RenderTarget()
{
	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
	{
		SAFE_RELEASE(&this->renderTargets[i]);
	}

	delete this->descriptorHeap;
}

DescriptorHeap* RenderTarget::GetDescriptorHeap()
{
	return this->descriptorHeap;
}

ID3D12Resource1** RenderTarget::GetRenderTarget(UINT index)
{
	return &this->renderTargets[index];
}

void RenderTarget::CreateViewport(unsigned int width, unsigned int height)
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

// TODO: Fler inparametrar sedan om man vill kunna customizea scizzorRecten
void RenderTarget::CreateScissorRect(unsigned int width, unsigned int height)
{
	scissorRect.left = (long)0;
	scissorRect.right = (long)width;
	scissorRect.top = (long)0;
	scissorRect.bottom = (long)height;
}

void RenderTarget::CreateDescriptorHeap(DESCRIPTOR_HEAP_TYPES type, UINT size)
{
	this->descriptorHeap = new DescriptorHeap(type, size);
}

D3D12_VIEWPORT* RenderTarget::GetViewPort()
{
	return &this->viewport;
}

D3D12_RECT* RenderTarget::GetScissorRect()
{
	return &this->scissorRect;
}
