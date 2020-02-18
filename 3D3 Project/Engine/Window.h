#ifndef WINDOW_H
#define WINDOW_H

#include "stdafx.h"

class Window
{
public:
	Window(HINSTANCE hInstance, int nCmdShow, int screenWidth, int screenHeight, bool fullScreen, LPCTSTR windowName, LPCTSTR windowTitle);
	~Window();

	bool ExitWindow();
	bool IsFullScreen();
	int GetScreenWidth();
	int GetScreenHeight();
	HWND* GetHwnd();
private:
	// Window * window;
	bool InitWindow(HINSTANCE hInstance, int nCmdShow);

	int screenWidth;
	int screenHeight;
	bool fullScreen;
	LPCTSTR windowName;
	LPCTSTR windowTitle;

	HWND hwnd;
};

#endif