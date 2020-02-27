#include "RenderTarget.h"

RenderTarget::RenderTarget()
{
}

RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(&this->renderTargetsHeap);
}

void RenderTarget::SetRTDescriptorSize(UINT size)
{
	this->renderTargetDescriptorSize = size;
}

UINT RenderTarget::GetRTDescriptorSize()
{
	return this->renderTargetDescriptorSize;
}

ID3D12DescriptorHeap** RenderTarget::GetRTHeap()
{
	return &this->renderTargetsHeap;
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

D3D12_VIEWPORT* RenderTarget::GetViewPort()
{
	return &this->viewport;
}

D3D12_RECT* RenderTarget::GetScissorRect()
{
	return &this->scissorRect;
}

	for (int i = 0; i < NUM_SWAP_BUFFERS; i++)
	{
		SAFE_RELEASE(&this->renderTargets[i]);
	}
	SAFE_RELEASE(this->renderTargets);