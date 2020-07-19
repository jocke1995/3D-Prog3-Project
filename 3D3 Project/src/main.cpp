#include "Engine/Renderer.h"
#include "Engine/Timer.h"
#include "Window.h"

// TODO: Problem med precompiled header eller liknande.. Intellisense problemet
#include "Engine/stdafx.h"

#include "ECS/SceneHandler.h"

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
    std::vector<Mesh*>* floorModel = renderer->LoadModel(L"Resources/Models/Floor/floor.obj");

#pragma region CreateScene1
    // Create Scene
    sceneHandler->CreateScene("scene1", renderer->GetCamera());

    Scene* scene = sceneHandler->GetScene("scene1");
    
    // Add Entity to Scene
    scene->AddEntity("floor");
    scene->AddEntity("cube1");
    scene->AddEntity("cube2");
    scene->AddEntity("cube3");
    scene->AddEntity("dragon");
    scene->AddEntity("mino1");
    scene->AddEntity("light");
    
    // Add Components to Entity
    scene->GetEntity("floor")->AddComponent<component::MeshComponent>();
    scene->GetEntity("floor")->AddComponent<component::TransformComponent>();
    scene->GetEntity("cube1")->AddComponent<component::MeshComponent>();
    scene->GetEntity("cube1")->AddComponent<component::TransformComponent>();
    scene->GetEntity("cube2")->AddComponent<component::MeshComponent>();
    scene->GetEntity("cube2")->AddComponent<component::TransformComponent>();
    scene->GetEntity("cube3")->AddComponent<component::MeshComponent>();
    scene->GetEntity("cube3")->AddComponent<component::TransformComponent>();
    scene->GetEntity("dragon")->AddComponent<component::MeshComponent>();
    scene->GetEntity("dragon")->AddComponent<component::TransformComponent>();
    scene->GetEntity("mino1")->AddComponent<component::MeshComponent>();
    scene->GetEntity("mino1")->AddComponent<component::TransformComponent>();
    scene->GetEntity("light")->AddComponent<component::MeshComponent>();
    scene->GetEntity("light")->AddComponent<component::TransformComponent>();
    scene->GetEntity("light")->AddComponent<component::DirectionalLightComponent>();
    
    // Set the components
    component::MeshComponent* mc = scene->GetEntity("floor")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(floorModel);
    mc->SetDrawFlag(DrawOptions::Blend);
    component::TransformComponent* tc = scene->GetEntity("floor")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(10);
    tc->GetTransform()->SetPosition(0.0f, -1.0f, 0.0f);

    mc = scene->GetEntity("cube1")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(DrawOptions::ForwardRendering);
    tc = scene->GetEntity("cube1")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.5);
    tc->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
    
    mc = scene->GetEntity("cube2")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(DrawOptions::Blend);
    tc = scene->GetEntity("cube2")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.57);
    tc->GetTransform()->SetPosition(0.0f, 0.0f, 10.0f);
    
    mc = scene->GetEntity("cube3")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(DrawOptions::ForwardRendering);
    tc = scene->GetEntity("cube3")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.5);
    tc->GetTransform()->SetPosition(0.0f, 0.0f, 20.0f);
    
    mc = scene->GetEntity("dragon")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(dragModel);
    mc->SetDrawFlag(DrawOptions::ForwardRendering);
    tc = scene->GetEntity("dragon")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.2);
    tc->GetTransform()->RotateX(3.0*DirectX::XM_PI / 2.0);
    tc->GetTransform()->SetPosition(30.0f, 0.0f, 30.0f);
    
    mc = scene->GetEntity("mino1")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(minoModel);
    mc->SetDrawFlag(DrawOptions::ForwardRendering);
    tc = scene->GetEntity("mino1")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.05);
    tc->GetTransform()->SetPosition(8.0f, 0.0f, 10.0f);
    //
    // --------------------------- Set Light components START---------------------------
    mc = scene->GetEntity("light")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(DrawOptions::ForwardRendering);
    tc = scene->GetEntity("light")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.2);
    tc->GetTransform()->SetPosition(0.0f, 5.0f, -5.0f);
    
    component::DirectionalLightComponent* dl = scene->GetEntity("light")->GetComponent<component::DirectionalLightComponent>();
    dl->SetLightFlag(LIGHT_FLAG::USE_MESH_POSITION);
    dl->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    dl->CreateResource(renderer->GetDevice());
    renderer->CreateConstantBufferView( dl->GetDescriptorHeapIndex(), 
                                        dl->GetCbSizeAligned(), 
                                        dl->GetResource());
    // --------------------------- Set Light components END---------------------------


#pragma endregion CreateScene1
    renderer->SetSceneToDraw(sceneHandler->GetScene("scene1"));

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
        //    scene1->GetEntity(entityName)->AddComponent<RenderCompoanent>();
        //    rc = scene1->GetEntity(entityName)->GetComponent<RenderCaomponent>();
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
        Transform* lt = scene->GetEntity("light")->GetComponent<component::TransformComponent>()->GetTransform();
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
