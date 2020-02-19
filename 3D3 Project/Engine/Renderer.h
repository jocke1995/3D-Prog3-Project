#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"	// Window has:  #include "stdafx.h"
#include "ConstantBuffer.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void CreateWindow(HINSTANCE hInstance, int nCmdShow, int screenWidth, int screenHeight, bool fullScreen, LPCTSTR windowName, LPCTSTR windowTitle);
	void InitD3D12();

	// TODO: Stefan vilka inparametrar är smart att ha med? Hela Heap Properties?
	ConstantBuffer& CreateConstantBuffer(std::wstring name, D3D12_HEAP_TYPE type, unsigned int width, unsigned int height, unsigned int depthOrArraySize);

	void Execute();

private:
	Window * window = nullptr;

	ID3D12Device5* device5 = nullptr;

	std::vector<ConstantBuffer*> constantBuffers;

	bool CreateDevice();
};

#endif