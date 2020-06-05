#include "Engine/Renderer.h"
#include "Engine/Camera.h"
#include "Window.h"

// Objects
#include "Minotaur.h"

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

    // The same mesh can be used for multiple objects
    Mesh* cubeMesh = renderer->CreateMesh(L"Resources/Models/mino.obj");

    // DrawFlags
    UINT drawOptionsFR = DrawOptions::ForwardRendering; // | DrawOptions::Shadow;
    UINT drawOptionsBlend = DrawOptions::Blend;

    // Unique For each object
    Object* mino1 = new Minotaur(cubeMesh, drawOptionsBlend);
    mino1->GetTransform()->SetPosition(0, 0, 0);

    Object* mino2 = new Minotaur(cubeMesh, drawOptionsBlend);
    mino2->GetTransform()->SetPosition(0, 0, 10);

    Object* mino3 = new Minotaur(cubeMesh, drawOptionsBlend);
    mino3->GetTransform()->SetPosition(0, 0, 20);

    Object* mino4 = new Minotaur(cubeMesh, drawOptionsBlend);
    mino4->GetTransform()->SetPosition(0, 0, 30);

    Object* mino5 = new Minotaur(cubeMesh, drawOptionsBlend);
    mino5->GetTransform()->SetPosition(0, 0, 40);

    renderer->AddObjectToTasks(mino1);
    renderer->AddObjectToTasks(mino2);
    renderer->AddObjectToTasks(mino3);
    renderer->AddObjectToTasks(mino4);
    renderer->AddObjectToTasks(mino5);
    renderer->UpdateObjectsToDraw();

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
        mino1->Update(dt);
        mino2->Update(dt);
        mino3->Update(dt);
        mino4->Update(dt);
        mino5->Update(dt);
        
        camera->Update(dt);

        renderer->SortObjectsByDistance(camera->GetPosition());
        renderer->UpdateObjectsToDraw();
        /* ------ Draw   ------ */
        renderer->Execute();
    }   

    // ---------------------------- SafeExit the program ----------------------------
    delete mino1;
    delete mino2;
    delete mino3;
    delete mino4;
    delete mino5;

    delete camera;
    delete window;
    delete renderer;
    return 0;
}

