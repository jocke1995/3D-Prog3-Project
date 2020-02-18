#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"	// Window has:  #include "stdafx.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void CreateWindow(HINSTANCE hInstance, int nCmdShow, int screenWidth, int screenHeight, bool fullScreen, LPCTSTR windowName, LPCTSTR windowTitle);
	void InitD3D12();

	void Run();

private:
	Window * window;

	void Update();
	void Draw();
};

#endif