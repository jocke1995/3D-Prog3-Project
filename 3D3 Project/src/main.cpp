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
    Camera* camera = new Camera(L"default_cam");

    // Test Mesh, kan användas till flera av "samma typ" objekt senare.
    Mesh* cubeMesh = renderer->CreateMesh(L"Resources/Models/cube3.obj");
    
    std::vector<Object*> objects;

    // Unique For each object
    Object* cube = new Cube(cubeMesh, 0);
    cube->GetTransform()->SetPosition(0, 0, 10);

    Object* cube2 = new Cube(cubeMesh, 1);
    cube2->GetTransform()->SetPosition(2, 0, 10);

    objects.push_back(cube);
    objects.push_back(cube2);

    renderer->SetObjectsToDraw(RenderTaskType::TEST, &objects);
    renderer->SetCamera(RenderTaskType::TEST, camera);

    // GAMELOOP
    // TODO : Add dt here
    while (!window->ExitWindow())
    {
        // Fill allocators etc...
        camera->Update(); // TODO: add dt
        cube->Update();   // TODO: add dt
        cube2->Update();   // TODO: add dt

        renderer->Execute();
    }   

    delete cube;
    delete cube2;
    delete camera;
    delete window;
    delete renderer;
    return 0;
}

