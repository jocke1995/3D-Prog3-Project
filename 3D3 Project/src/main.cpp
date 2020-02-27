#include "Engine/Renderer.h"
#include "AssetLoader.h"
#include "Window.h"

#include "Engine/Transform.h"
#include "Engine/Cube.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Window* window = new Window(hInstance, nCmdShow, 800, 600, false, L"windowName", L"windowTitle");
    Renderer* renderer = new Renderer();
 
    renderer->InitD3D12(window->GetHwnd());

    RenderTask* testTask = new RenderTaskTest();
    testTask->AddRenderTarget(renderer->GetRenderTarget(RenderTargetTypes::SWAPCHAIN, 0));

    renderer->AddRenderTask(testTask);

    // Test
    Mesh* a = AssetLoader::Get().LoadMesh(L"Resources/Models/cube3.obj");

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

