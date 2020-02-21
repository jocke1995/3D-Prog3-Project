#include "Engine/Renderer.h"

// TODO: kolla upp hur vi ska g�ra med dessa includes senare
#include "Engine/RenderTaskTest.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Renderer* renderer = new Renderer();
    renderer->CreateWindow(hInstance, nCmdShow, 800, 600, false, L"Windowname", L"windowTitle");
    renderer->InitD3D12();

    RenderTask* testTask = new RenderTaskTest();
    renderer->CreateRenderTask(testTask);
    renderer->AddRenderTask(testTask);
    
    // Fill allocators etc...
    renderer->Execute();   

    delete testTask;
    delete renderer;
    return 0;
}

