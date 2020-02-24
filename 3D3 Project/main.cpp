#include "Engine/Renderer.h"
#include "AssetLoader.h"

#include "Engine/Transform.h"
#include "Engine/Cube.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Renderer* renderer = new Renderer();
    renderer->CreateWindow(hInstance, nCmdShow, 800, 600, false, L"Windowname", L"windowTitle");
    renderer->InitD3D12();
    
    // Testar CB
    ConstantBuffer* CB = renderer->CreateConstantBuffer(L"test", D3D12_HEAP_TYPE_UPLOAD, 10, CONSTANT_BUFFER_TYPE::CB_PER_OBJECT);

    Cube* c = new Cube(CB);

    //AssetLoader::Get().LoadMesh(L"hej");

    // Fill allocators etc...
    renderer->Execute();   

    delete renderer;
    return 0;
}

