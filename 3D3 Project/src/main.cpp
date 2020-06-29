#include "Engine/Renderer.h"
#include "Engine/Timer.h"
#include "Window.h"

// TODO: Problem med precompiled header eller liknande.. Intellisense problemet
#include "Engine/stdafx.h"

#include "Game/Scene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    /* ------ Window  ------ */
    Window* window = new Window(hInstance, nCmdShow, 800, 600, false, L"windowName", L"windowTitle");

    /* ------ Timer  ------ */
    Timer* timer = new Timer(window);

    /* ------ Renderer  ------ */
    Renderer* renderer = new Renderer();
    renderer->InitD3D12(window->GetHwnd(), hInstance);

    // Get threadpool so other tasks (physics, gameupdates etc..) can use it 
    ThreadPool* threadPool = renderer->GetThreadPool();

    // This will be loaded once from disk, then the next time the same function is called (with the same filepath),
    // the function will just return the same pointer to the mesh that was loaded earlier.
    std::vector<Mesh*>* minoModel = renderer->LoadModel(L"Resources/Models/mino.obj");
    std::vector<Mesh*>* cubeModel = renderer->LoadModel(L"Resources/Models/cube.obj");
    std::vector<Mesh*>* dragModel = renderer->LoadModel(L"Resources/Models/dragon.fbx");

#pragma region CreateScene1

    // Create Scene
    Scene* scene1 = new Scene(renderer->GetCamera());

    // Add Entity to Scene
    scene1->AddEntity("mino1");
    scene1->AddEntity("mino2");
    scene1->AddEntity("mino3");
    scene1->AddEntity("mino4");
    
    // Add Components to Entity
    scene1->GetEntity("mino1")->AddComponent<RenderComponent>();
    scene1->GetEntity("mino2")->AddComponent<RenderComponent>();
    scene1->GetEntity("mino3")->AddComponent<RenderComponent>();
    scene1->GetEntity("mino4")->AddComponent<RenderComponent>();

    // Set the components
    RenderComponent* rc = scene1->GetEntity("mino1")->GetComponent<RenderComponent>();
    rc->SetMeshes(minoModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
    
    rc = scene1->GetEntity("mino2")->GetComponent<RenderComponent>();
    rc->SetMeshes(minoModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 10.0f);
    
    rc = scene1->GetEntity("mino3")->GetComponent<RenderComponent>();
    rc->SetMeshes(minoModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 20.0f);
    
    rc = scene1->GetEntity("mino4")->GetComponent<RenderComponent>();
    rc->SetMeshes(minoModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 30.0f);

#pragma endregion CreateScene1

#pragma region CreateScene2
    // Create Scene
    Scene* scene2 = new Scene(renderer->GetCamera());
    
    // Add Entity to Scene
    scene2->AddEntity("cube1");
    scene2->AddEntity("cube2");
    scene2->AddEntity("cube3");
    scene2->AddEntity("dragon");
    scene2->AddEntity("mino1");
    scene2->AddEntity("light");
    
    // Add Components to Entity
    scene2->GetEntity("cube1")->AddComponent<RenderComponent>();
    scene2->GetEntity("cube2")->AddComponent<RenderComponent>();
    scene2->GetEntity("cube3")->AddComponent<RenderComponent>();
    scene2->GetEntity("dragon")->AddComponent<RenderComponent>();
    scene2->GetEntity("mino1")->AddComponent<RenderComponent>();
    scene2->GetEntity("light")->AddComponent<RenderComponent>();
    
    // Set the components
    rc = scene2->GetEntity("cube1")->GetComponent<RenderComponent>();
    rc->SetMeshes(cubeModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.5);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
    
    rc = scene2->GetEntity("cube2")->GetComponent<RenderComponent>();
    rc->SetMeshes(minoModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 10.0f);
    
    rc = scene2->GetEntity("cube3")->GetComponent<RenderComponent>();
    rc->SetMeshes(cubeModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.5);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 20.0f);
    
    rc = scene2->GetEntity("dragon")->GetComponent<RenderComponent>();
    rc->SetMeshes(dragModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.2);
    rc->GetTransform()->RotateX(3.0*DirectX::XM_PI / 2.0);
    rc->GetTransform()->SetPosition(30.0f, 0.0f, 30.0f);
    
    rc = scene2->GetEntity("mino1")->GetComponent<RenderComponent>();
    rc->SetMeshes(cubeModel);
    rc->SetDrawFlag(DrawOptions::Blend);
    rc->GetTransform()->SetScale(0.57);
    rc->GetTransform()->SetPosition(8.0f, 0.0f, 10.0f);
    
    rc = scene2->GetEntity("light")->GetComponent<RenderComponent>();
    rc->SetMeshes(cubeModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.2);
    rc->GetTransform()->SetPosition(3.0f, 5.0f, -5.0f);

#pragma endregion CreateScene2
    renderer->SetSceneToDraw(scene2);

    while (!window->ExitWindow())
    {  
        /* ------ Timer  ------ */
        timer->Update();

        // Test to add objects during runtime
        //if (window->WasSpacePressed())
        //{
        //    char entityName[10];
        //    static int entityCounter = 0;
        //    sprintf(entityName, "cube%d", entityCounter);
        //
        //    entityCounter++;
        //
        //    // Test to add entity during runtime
        //    scene1->AddEntity(entityName);
        //    scene1->GetEntity(entityName)->AddComponent<RenderComponent>();
        //    rc = scene1->GetEntity(entityName)->GetComponent<RenderComponent>();
        //    rc->SetMesh(cubeMesh);
        //    rc->SetDrawFlag(DrawOptions::ForwardRendering);
        //    
        //    // Create entity infront of camera
        //    rc->GetTransform()->SetPosition(scene1->GetMainCamera()->GetPosition().x + scene1->GetMainCamera()->GetLookAt().x*8,
        //                                    scene1->GetMainCamera()->GetPosition().y + scene1->GetMainCamera()->GetLookAt().y*8,
        //                                    scene1->GetMainCamera()->GetPosition().z + scene1->GetMainCamera()->GetLookAt().z*8);
        //
        //    renderer->AddEntityToDraw(scene1->GetEntity(entityName));
        //
        //    // Test To Remove Entity
        //    //renderer->RemoveEntityFromDraw(scene1->GetEntity(entityName2));
        //    //char entityName2[10];
        //    //sprintf(entityName2, "mino%d", entityCounter);
        //    //scene1->RemoveEntity(entityName2);
        //} 

        /* ------ Update ------ */
        renderer->UpdateScene(timer->GetDeltaTime());

        renderer->SortEntitiesByDistance();

        /* ------ Draw   ------ */
        renderer->Execute();
    }

    delete scene1;
    delete scene2;
    delete window;
    delete renderer;
    delete timer;

    return 0;
}
