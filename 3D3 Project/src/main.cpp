#include "Engine/Renderer.h"
#include "Engine/Timer.h"

#include "ECS/SceneHandler.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    /* ------ Window  ------ */
    Window* window = new Window(hInstance, nCmdShow);

    /* ------ Timer  ------ */
    Timer* timer = new Timer(window);

    /* ------ Renderer  ------ */
    Renderer renderer = Renderer();
    renderer.InitD3D12(window->GetHwnd(), hInstance);

    // Get threadpool so other tasks (physics, gameupdates etc..) can use it 
    ThreadPool* threadPool = renderer.GetThreadPool();

    // Handler to the scenes, which will be used to create different scenes with entities..
    SceneHandler* sceneHandler = new SceneHandler();

    // This will be loaded once from disk, then the next time the same function is called (with the same filepath),
    // the function will just return the same pointer to the model that was loaded earlier.
    std::vector<Mesh*>* floorModel = renderer.LoadModel(L"Resources/Models/Floor/floor.obj");
    std::vector<Mesh*>* stoneModel = renderer.LoadModel(L"Resources/Models/Rock/rock.obj");
    std::vector<Mesh*>* cubeModel  = renderer.LoadModel(L"Resources/Models/Cube/crate.obj");

#pragma region CreateScene0
    // Create Scene
    sceneHandler->CreateScene("scene0", renderer.GetCamera());

    Scene* scene = sceneHandler->GetScene("scene0");
    
    // Add Entity to Scene
    scene->AddEntity("floor");
    scene->AddEntity("box");
    scene->AddEntity("stone");
    scene->AddEntity("transparentTestObject");
    scene->AddEntity("directionalLight");
    scene->AddEntity("spotLight");
    scene->AddEntity("spotLight2");

    // Add Components to Entities
    scene->GetEntity("floor")->AddComponent<component::MeshComponent>();
    scene->GetEntity("floor")->AddComponent<component::TransformComponent>();
    scene->GetEntity("box")->AddComponent<component::MeshComponent>();
    scene->GetEntity("box")->AddComponent<component::TransformComponent>();
    scene->GetEntity("stone")->AddComponent<component::MeshComponent>();
    scene->GetEntity("stone")->AddComponent<component::TransformComponent>();
    scene->GetEntity("transparentTestObject")->AddComponent<component::MeshComponent>();
    scene->GetEntity("transparentTestObject")->AddComponent<component::TransformComponent>();
    scene->GetEntity("directionalLight")->AddComponent<component::DirectionalLightComponent>(FLAG_LIGHT::CAST_SHADOW_LOW_RESOLUTION);
    scene->GetEntity("spotLight")->AddComponent<component::SpotLightComponent>(FLAG_LIGHT::CAST_SHADOW_LOW_RESOLUTION);

    // Set the components
    component::MeshComponent* mc = scene->GetEntity("floor")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(floorModel);
    mc->SetDrawFlag(FLAG_DRAW::ForwardRendering | FLAG_DRAW::Shadow);
    component::TransformComponent* tc = scene->GetEntity("floor")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(20, 1, 20);
    tc->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);

    mc = scene->GetEntity("box")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(FLAG_DRAW::ForwardRendering | FLAG_DRAW::Shadow);

    tc = scene->GetEntity("box")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.5f);
    tc->GetTransform()->SetPosition(32.0f, 1.0f, 20.0f);

    mc = scene->GetEntity("stone")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(stoneModel);
    mc->SetDrawFlag(FLAG_DRAW::ForwardRendering | FLAG_DRAW::Shadow);
    tc = scene->GetEntity("stone")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.01f);
    tc->GetTransform()->SetPosition(-8.0f, 0.0f, 0.0f);

    mc = scene->GetEntity("transparentTestObject")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(floorModel);
    mc->SetDrawFlag(FLAG_DRAW::Blend);

    Texture* ambientDefault = renderer.LoadTexture(L"Resources/Textures/Default/default_ambient.png");
    Texture* normalDefault = renderer.LoadTexture(L"Resources/Textures/Default/default_normal.png");
    mc->GetMesh(0)->SetTexture(TEXTURE_TYPE::AMBIENT , ambientDefault);
    mc->GetMesh(0)->SetTexture(TEXTURE_TYPE::DIFFUSE , ambientDefault);
    mc->GetMesh(0)->SetTexture(TEXTURE_TYPE::SPECULAR, ambientDefault);
    mc->GetMesh(0)->SetTexture(TEXTURE_TYPE::NORMAL  , normalDefault);

    tc = scene->GetEntity("transparentTestObject")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(5.0f);
    tc->GetTransform()->SetPosition(0.0f, 5.0f, 1.0f);
    tc->GetTransform()->RotateZ(3.141572f / 2.0f);
    tc->GetTransform()->RotateX(3.141572f / 2.0f);

    component::DirectionalLightComponent* dl = scene->GetEntity("directionalLight")->GetComponent<component::DirectionalLightComponent>();
    dl->SetDirection({ -1.0f, -1.0f, -1.0f });

    component::SpotLightComponent* sl = scene->GetEntity("spotLight")->GetComponent<component::SpotLightComponent>();
    sl->SetPosition({ -20.0f, 6.0f, -3.0f });
    sl->SetDirection({ 2.0, -1.0, 0.0f });
    sl->SetColor(LIGHT_COLOR_TYPE::LIGHT_DIFFUSE, { 1.0f, 0.0f, 0.0f, 1.0f });
    sl->SetColor(LIGHT_COLOR_TYPE::LIGHT_AMBIENT, { 0.005f, 0.005f, 0.005f, 1.0f });
    sl->SetColor(LIGHT_COLOR_TYPE::LIGHT_SPECULAR, { 1.0f, 0.0f, 0.0f, 1.0f });

#pragma endregion CreateScene0
#pragma region CreateScene1
    // Create Scene
    sceneHandler->CreateScene("scene1", renderer.GetCamera());
    scene = sceneHandler->GetScene("scene1");
    scene->AddEntity("cube1");
    scene->AddEntity("cube2");
    scene->AddEntity("cube3");
    scene->GetEntity("cube1")->AddComponent<component::MeshComponent>();
    scene->GetEntity("cube1")->AddComponent<component::TransformComponent>();
    scene->GetEntity("cube2")->AddComponent<component::MeshComponent>();
    scene->GetEntity("cube2")->AddComponent<component::TransformComponent>();
    scene->GetEntity("cube3")->AddComponent<component::MeshComponent>();
    scene->GetEntity("cube3")->AddComponent<component::TransformComponent>();
    scene->GetEntity("cube3")->AddComponent<component::DirectionalLightComponent>(FLAG_LIGHT::CAST_SHADOW_LOW_RESOLUTION);
    
    mc = scene->GetEntity("cube1")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(FLAG_DRAW::ForwardRendering | FLAG_DRAW::Shadow);
    
    tc = scene->GetEntity("cube1")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.5f);
    tc->GetTransform()->SetPosition(-15.0f, 1.0f, 0.0f);
    
    mc = scene->GetEntity("cube2")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(FLAG_DRAW::ForwardRendering | FLAG_DRAW::Shadow);
    
    tc = scene->GetEntity("cube2")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.5f);
    tc->GetTransform()->SetPosition(-5.0f, 1.0f, 0.0f);
    
    mc = scene->GetEntity("cube3")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(FLAG_DRAW::ForwardRendering | FLAG_DRAW::Shadow);
    
    tc = scene->GetEntity("cube3")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.5f);
    tc->GetTransform()->SetPosition(10.0f, 5.0f, 5.0f);
   
    //dl = scene->GetEntity("cube3")->GetComponent<component::DirectionalLightComponent>();
    //dl->Init();
    //dl->SetDirection({ -1.0f, -0.5f, -0.5f });

#pragma endregion CreateScene1
    renderer.SetSceneToDraw(sceneHandler->GetScene("scene0"));
    while (!window->ExitWindow())
    {
        if (window->WasSpacePressed())
        {
            //Test to change scene during runtime
            //char sceneName[10];
            //static int sceneSwapper = 1;
            //sceneSwapper %= 2;
            //sprintf(sceneName, "scene%d", sceneSwapper);
            //renderer.SetSceneToDraw(sceneHandler->GetScene(sceneName));
            //sceneSwapper++;

            scene = sceneHandler->GetScene("scene0");
            tc = scene->GetEntity("stone")->GetComponent<component::TransformComponent>();
            float3 posa = tc->GetTransform()->GetPositionFloat3();
            tc->GetTransform()->SetPosition(posa.x, posa.y, posa.z + 0.1);


        // Test to add objects during runtime
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
        } 

        /* ------ Update ------ */
        timer->Update();
        renderer.Update(timer->GetDeltaTime());

        /* ------ Sort ------ */
        renderer.SortObjectsByDistance();

        /* ------ Draw ------ */
        renderer.Execute();
    }

    delete window;
    delete sceneHandler;
    delete timer;

    return 0;
}
