#include "Engine/Renderer.h"
#include "Window.h"
#include <chrono>
#include <ctime>

#include "Engine/Cube.h"
#include "Engine/Camera.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Setup timer
    auto start = std::chrono::system_clock::now();

    Window* window = new Window(hInstance, nCmdShow, 800, 600, false, L"windowName", L"windowTitle");
    Renderer* renderer = new Renderer();
    renderer->InitD3D12(window->GetHwnd());
    renderer->InitRenderTasks();

    // Camera
    Camera* camera = new Camera(L"default_cam", hInstance, *window->GetHwnd());

    // Test Mesh, kan anv�ndas till flera av "samma typ" objekt senare.
    Mesh* cubeMesh = renderer->CreateMesh(L"Resources/Models/cube3.obj");
    
    std::vector<Object*> objects;

    // Unique For each object
    Object* cube = new Cube(cubeMesh, 0);
    cube->GetTransform()->SetPosition(-2, 0, 10);

    Object* cube2 = new Cube(cubeMesh, 1);
    cube2->GetTransform()->SetPosition(2, 0, 10);

    objects.push_back(cube);
    objects.push_back(cube2);

    renderer->SetObjectsToDraw(RenderTaskType::TEST, &objects);
    renderer->SetCamera(RenderTaskType::TEST, camera);

    // GAMELOOP
    auto time_now = start;
    while (!window->ExitWindow())
    {
        /* ------ Timer  ------ */
        // TODO: Create Timer Class
        auto time_last = time_now;
        time_now = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = time_now - time_last;
        double dt = elapsed_time.count(); // dt.count() to get ms time
        
        //OutputDebugStringW(std::to_wstring(dt.count()).c_str());

        /* ------ Update ------ */
        cube->Update(dt);
        cube2->Update(dt);
        
        /* ------ Draw   ------ */
        camera->Update(dt);
        renderer->Execute();
    }   

    for (Object* object : objects)
        delete object;

    delete camera;
    delete window;
    delete renderer;
    return 0;
}

