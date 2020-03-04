#include "Engine/Renderer.h"
#include "AssetLoader.h"
#include "Window.h"

#include "Engine/Transform.h"
#include "Engine/Cube.h"
#include "Engine/Camera.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Window* window = new Window(hInstance, nCmdShow, 800, 600, false, L"windowName", L"windowTitle");
    Renderer* renderer = new Renderer();
    renderer->InitD3D12(window->GetHwnd());
    renderer->InitRenderTasks();

    // Camera
    ConstantBuffer* cameraConstantBuffer = renderer->GetConstantBuffer(CONSTANT_BUFFER_TYPE::CB_CAMERA);
    Camera* camera = new Camera(L"default_cam", cameraConstantBuffer);

    // Test Mesh, kan anv�ndas till flera av "samma typ" objekt senare.
    Mesh* cubeMesh = AssetLoader::Get().LoadMesh(L"Resources/Models/cube3.obj");
    renderer->CreateVertexBuffer(cubeMesh);

    // Unique For each object
    ConstantBuffer* transformConstantBuffer = renderer->GetConstantBuffer(CONSTANT_BUFFER_TYPE::CB_PER_OBJECT);
    Object* cube = new Cube(transformConstantBuffer, cubeMesh);

    cube->GetTransform()->SetScale(1, 1, 1);

    std::vector<Object*> objects;
    objects.push_back(cube);

    renderer->SetObjectsToDraw(RenderTaskType::TEST, &objects);

    // GAMELOOP
    while (!window->ExitWindow())
    {
        // Fill allocators etc...
        renderer->Execute();
    }   

    delete cube;
    delete camera;
    delete window;
    delete renderer;
    return 0;
}

