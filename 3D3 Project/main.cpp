#include "Engine/Renderer.h"
#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Window* window = new Window(hInstance, nCmdShow, 800, 600, false, L"windowName", L"windowTitle");
    Renderer* renderer = new Renderer();

    renderer->InitD3D12(window->GetHwnd());

    RenderTask* testTask = new RenderTaskTest();
    renderer->AddRenderTask(testTask);

    testTask->AddRenderTarget(renderer->GetRenderTarget(RenderTargetTypes::SWAPCHAIN, 0));

    // GAMELOOP
    while (!window->ExitWindow())
    {
        // Fill allocators etc...
        renderer->Execute();
    }   

    delete window;
    delete testTask;
    delete renderer;
    return 0;
}

