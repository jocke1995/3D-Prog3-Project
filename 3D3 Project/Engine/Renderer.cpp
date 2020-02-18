#include "Renderer.h"

Renderer::Renderer()
{
	this->window = nullptr;
}

Renderer::~Renderer()
{
	delete this->window;
}

void Renderer::CreateWindow(HINSTANCE hInstance, int nCmdShow, int screenWidth, int screenHeight, bool fullScreen, LPCTSTR windowName, LPCTSTR windowTitle)
{
	this->window = new Window(hInstance, nCmdShow, screenWidth, screenHeight, fullScreen, windowName, windowTitle);
}

void Renderer::InitD3D12()
{
	// Init DX stuff... device, swapchain etc..
}

void Renderer::Run()
{
	while (!this->window->ExitWindow())
	{
		this->Update();

		this->Draw();
	}
}

// -----------------------  Private Functions  ----------------------- //
void Renderer::Update()
{

}

void Renderer::Draw()
{

}