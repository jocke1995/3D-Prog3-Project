#include "Engine/Renderer.h"
#include "Engine/Camera.h"
#include "Window.h"

// Problem med precompiled header eller liknande.. Intellisense problemet
#include "Engine/stdafx.h"

#include "Game/Scene.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Setup timer
    auto start = std::chrono::system_clock::now();

    Window* window = new Window(hInstance, nCmdShow, 800, 600, false, L"windowName", L"windowTitle");

    Renderer* renderer = new Renderer();
    renderer->InitD3D12(window->GetHwnd());
    renderer->InitRenderTasks();

    // Get threadpool so other tasks (physics, gameupdates etc..) can use it 
    ThreadPool* threadPool = renderer->GetThreadPool();
    
    // Camera
    Camera* camera = new Camera(L"default_cam", hInstance, *window->GetHwnd());
    renderer->SetCamera(camera);

    // The same mesh can be used for multiple Entities
    Mesh* minoMesh = renderer->CreateMesh(L"Resources/Models/mino.obj");

    // DrawFlags
    UINT drawOptionsFR = DrawOptions::ForwardRendering; // | DrawOptions::Shadow;
    UINT drawOptionsBlend = DrawOptions::Blend;

    // --------------------------------------------------------------------------------

    // Create Scene
    Scene* scene1 = new Scene();
    
    // Add Entity to Scene
    scene1->AddEntity("mino1");
    
    // Add Components to Entity
    scene1->GetEntity("mino1")->AddComponent<HealthComponent>();
    scene1->GetEntity("mino1")->AddComponent<RenderComponent>();

    // Set the components
    RenderComponent* rc = scene1->GetEntity("mino1")->GetComponent<RenderComponent>();
    rc->SetMesh(minoMesh);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);

    renderer->SetSceneToDraw(scene1);

    // --------------------------------------------------------------------------------

    // GAMELOOP
    auto time_now = start;
    auto time_lastFps = start;
    double fpsInterval = 0.5;

    while (!window->ExitWindow())
    {
        /* ------ Timer  ------ */
        // TODO: Create Timer Class
        auto time_last = time_now;
        time_now = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = time_now - time_last;
        double dt = elapsed_time.count(); // dt.count() to get ms time
        
        std::chrono::duration<double> elapsed_timeFps = time_now - time_lastFps;
        if (elapsed_timeFps.count() >= fpsInterval)
        {
            std::string fpsString = std::to_string(int(1.0 / dt));
            std::wstring tmp = std::wstring(fpsString.begin(), fpsString.end());
            window->SetWindowTitle(tmp);
            time_lastFps = time_now;
        }

        /* ------ Update ------ */
        scene1->UpdateEntities();
        
        camera->Update(dt);

        //renderer->SortObjectsByDistance(camera->GetPosition());
        //renderer->UpdateObjectsToDraw();
        /* ------ Draw   ------ */
        renderer->Execute();
    }   

    // ---------------------------- SafeExit the program ----------------------------

    delete scene1;

    delete camera;
    delete window;
    delete renderer;
    return 0;
}

