#include "Engine/Renderer.h"
#include "Engine/Timer.h"
#include "Window.h"

// TODO: Problem med precompiled header eller liknande.. Intellisense problemet
#include "Engine/stdafx.h"

#include "Game/SceneHandler.h"

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

    // Handler to the scenes, which will be used to create different scenes with entities..
    SceneHandler* sceneHandler = new SceneHandler();

    // This will be loaded once from disk, then the next time the same function is called (with the same filepath),
    // the function will just return the same pointer to the mesh that was loaded earlier.
    std::vector<Mesh*>* minoModel = renderer->LoadModel(L"Resources/Models/mino.obj");
    std::vector<Mesh*>* cubeModel = renderer->LoadModel(L"Resources/Models/cube.obj");
    std::vector<Mesh*>* dragModel = renderer->LoadModel(L"Resources/Models/dragon.fbx");

    
#pragma region CreateScene1

    // Create Scene
    sceneHandler->CreateScene("scene1", renderer->GetCamera());

    Scene* scene = sceneHandler->GetScene("scene1");
    // Add Entity to Scene
    scene->AddEntity("mino1");
    scene->AddEntity("mino2");
    scene->AddEntity("mino3");
    scene->AddEntity("mino4");
    
    // Add Components to Entity
    scene->GetEntity("mino1")->AddComponent<component::RenderComponent>();
    scene->GetEntity("mino2")->AddComponent<component::RenderComponent>();
    scene->GetEntity("mino3")->AddComponent<component::RenderComponent>();
    scene->GetEntity("mino4")->AddComponent<component::RenderComponent>();

    // Set the components
    component::RenderComponent* rc = scene->GetEntity("mino1")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(minoModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
    
    rc = scene->GetEntity("mino2")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(minoModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 10.0f);
    
    rc = scene->GetEntity("mino3")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(minoModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 20.0f);
    
    rc = scene->GetEntity("mino4")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(minoModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 30.0f);

#pragma endregion CreateScene1

#pragma region CreateScene2
    // Create Scene
    sceneHandler->CreateScene("scene2", renderer->GetCamera());

    scene = sceneHandler->GetScene("scene2");
    
    // Add Entity to Scene
    scene->AddEntity("Floor");
    scene->AddEntity("cube1");
    scene->AddEntity("cube2");
    scene->AddEntity("cube3");
    scene->AddEntity("dragon");
    scene->AddEntity("mino1");
    scene->AddEntity("light");
    
    // Add Components to Entity
    scene->GetEntity("Floor")->AddComponent<component::RenderComponent>();
    scene->GetEntity("cube1")->AddComponent<component::RenderComponent>();
    scene->GetEntity("cube2")->AddComponent<component::RenderComponent>();
    scene->GetEntity("cube3")->AddComponent<component::RenderComponent>();
    scene->GetEntity("dragon")->AddComponent<component::RenderComponent>();
    scene->GetEntity("mino1")->AddComponent<component::RenderComponent>();
    scene->GetEntity("light")->AddComponent<component::RenderComponent>();
    //scene->GetEntity("light")->AddComponent<component::DirectionalLightComponent>();
    
    // Set the components
    rc = scene->GetEntity("Floor")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(cubeModel);
    rc->SetDrawFlag(DrawOptions::Blend);
    rc->GetTransform()->SetScale(50);
    rc->GetTransform()->SetPosition(0.0f, -53.0f, 0.0f);

    rc = scene->GetEntity("cube1")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(cubeModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.5);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
    
    rc = scene->GetEntity("cube2")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(cubeModel);
    rc->SetDrawFlag(DrawOptions::Blend);
    rc->GetTransform()->SetScale(0.57);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 10.0f);
    
    rc = scene->GetEntity("cube3")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(cubeModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.5);
    rc->GetTransform()->SetPosition(0.0f, 0.0f, 20.0f);
    
    rc = scene->GetEntity("dragon")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(dragModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.2);
    rc->GetTransform()->RotateX(3.0*DirectX::XM_PI / 2.0);
    rc->GetTransform()->SetPosition(30.0f, 0.0f, 30.0f);
    
    rc = scene->GetEntity("mino1")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(minoModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.05);
    rc->GetTransform()->SetPosition(8.0f, 0.0f, 10.0f);
    
    // --------------------------- Set Light components START---------------------------
    rc = scene->GetEntity("light")->GetComponent<component::RenderComponent>();
    rc->SetMeshes(cubeModel);
    rc->SetDrawFlag(DrawOptions::ForwardRendering);
    rc->GetTransform()->SetScale(0.2);
    rc->GetTransform()->SetPosition(0.0f, 5.0f, -5.0f);

    //component::DirectionalLightComponent* dl = scene->GetEntity("light")->GetComponent<component::DirectionalLightComponent>();
    //
    //dl->SetLightFlag(LIGHT_FLAG::USE_MESH_POSITION);
    //dl->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    //dl->CreateResource(renderer->GetDevice());
    //renderer->CreateConstantBufferView( dl->GetDescriptorHeapIndex(), 
    //                                    dl->GetCbSizeAligned(), 
    //                                    dl->GetResource());
    // --------------------------- Set Light components END---------------------------


#pragma endregion CreateScene2
    renderer->SetSceneToDraw(sceneHandler->GetScene("scene2"));

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
        static double x = 0.0;
        x += 0.005f;
        Transform* lt = scene->GetEntity("light")->GetComponent<component::RenderComponent>()->GetTransform();
        lt->SetPosition(lt->GetPositionFloat3().x + sin(x) * timer->GetDeltaTime() * 10, lt->GetPositionFloat3().y, lt->GetPositionFloat3().z);
        renderer->UpdateScene(timer->GetDeltaTime());

        renderer->SortEntitiesByDistance();
        /* ------ Draw   ------ */
        renderer->Execute();
    }

    delete window;
    delete renderer;
    delete sceneHandler;
    delete timer;

    return 0;
}
