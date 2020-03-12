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
    renderer->SetCamera(RenderTaskType::TEST, camera);
    renderer->SetCamera(RenderTaskType::BLEND, camera);

    // The same mesh can be used for multiple objects
    Mesh* cubeMesh = renderer->CreateMesh(L"Resources/Models/cube3.obj");

    // TODO: STEFAN, måsvingar?
    DrawOptions drawOptionsTest;
    drawOptionsTest.test = true;
    DrawOptions drawOptionsBlend;
    drawOptionsBlend.blend = true;

    // Unique For each object
    Object* cube = new Cube(cubeMesh, &drawOptionsTest);
    cube->GetTransform()->SetPosition(-2, 0, 10);

    Object* cube2 = new Cube(cubeMesh, &drawOptionsBlend);
    cube2->GetTransform()->SetPosition(2, 0, 10);

    renderer->AddObjectToTasks(cube);
    renderer->AddObjectToTasks(cube2);

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
        
        camera->Update(dt);

        /* ------ Draw   ------ */
        renderer->Execute();
    }   

    delete cube;
    delete cube2;

    delete camera;
    delete window;
    delete renderer;
    return 0;
}

