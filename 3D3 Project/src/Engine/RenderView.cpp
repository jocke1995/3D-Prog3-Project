#include "RenderView.h"

RenderView::RenderView(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;

	this->CreateViewport();
	this->CreateScissorRect();
}

RenderView::~RenderView()
{
}

const D3D12_VIEWPORT* RenderView::GetViewPort() const
{
	return &this->viewport;
}

const D3D12_RECT* RenderView::GetScissorRect() const
{
	return &this->scissorRect;
}

void RenderView::CreateViewport()
{
	this->viewport.TopLeftX = 0.0f;
	this->viewport.TopLeftY = 0.0f;
	this->viewport.Width = (float)this->width;
	this->viewport.Height = (float)this->height;
	this->viewport.MinDepth = 0.0f;
	this->viewport.MaxDepth = 1.0f;
}

void RenderView::CreateScissorRect()
{
	this->scissorRect.left = (long)0;
	this->scissorRect.right = (long)this->width;
	this->scissorRect.top = (long)0;
	this->scissorRect.bottom = (long)this->height;
}
