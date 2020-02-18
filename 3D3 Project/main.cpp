#include "Engine/Renderer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Renderer* renderer = new Renderer();
    renderer->CreateWindow(hInstance, nCmdShow, 800, 600, false, L"Windowname", L"windowTitle");
    renderer->InitD3D12();
    
    // Fill allocators etc...
    renderer->Execute();   

    delete renderer;
    return 0;
}

