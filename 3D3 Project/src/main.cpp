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
    scene->AddEntity("stone");
    scene->AddEntity("transparentTestObject");
    scene->AddEntity("directionalLight");
    scene->AddEntity("spotLight");
    
    // Add Components to Entities
    scene->GetEntity("floor")->AddComponent<component::MeshComponent>();
    scene->GetEntity("floor")->AddComponent<component::TransformComponent>();
    scene->GetEntity("stone")->AddComponent<component::MeshComponent>();
    scene->GetEntity("stone")->AddComponent<component::TransformComponent>();
    scene->GetEntity("transparentTestObject")->AddComponent<component::MeshComponent>();
    scene->GetEntity("transparentTestObject")->AddComponent<component::TransformComponent>();
    scene->GetEntity("directionalLight")->AddComponent<component::DirectionalLightComponent>(LIGHT_FLAG::CAST_SHADOW);
    scene->GetEntity("spotLight")->AddComponent<component::MeshComponent>();
    scene->GetEntity("spotLight")->AddComponent<component::TransformComponent>();
    scene->GetEntity("spotLight")->AddComponent<component::SpotLightComponent>(LIGHT_FLAG::USE_TRANSFORM_POSITION);
    
    // Set the components
    component::MeshComponent* mc = scene->GetEntity("floor")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(floorModel);
    mc->SetDrawFlag(DRAW_FLAG::ForwardRendering | DRAW_FLAG::Shadow);
    component::TransformComponent* tc = scene->GetEntity("floor")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(15, 1, 15);
    tc->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);

    mc = scene->GetEntity("stone")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(stoneModel);
    mc->SetDrawFlag(DRAW_FLAG::ForwardRendering | DRAW_FLAG::Shadow);
    tc = scene->GetEntity("stone")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.005f);
    tc->GetTransform()->SetPosition(-8.0f, 0.0f, 0.0f);

    mc = scene->GetEntity("transparentTestObject")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(floorModel);
    mc->SetDrawFlag(DRAW_FLAG::Blend | DRAW_FLAG::Shadow);

    Texture* ambientDefault = renderer.LoadTexture(L"Resources/Textures/Default/default_diffuse.jpg");
    Texture* normalDefault = renderer.LoadTexture(L"Resources/Textures/Default/default_normal.png");
    mc->GetMesh(0)->SetTexture(TEXTURE_TYPE::AMBIENT , ambientDefault);
    mc->GetMesh(0)->SetTexture(TEXTURE_TYPE::DIFFUSE , ambientDefault);
    mc->GetMesh(0)->SetTexture(TEXTURE_TYPE::SPECULAR, ambientDefault);
    mc->GetMesh(0)->SetTexture(TEXTURE_TYPE::NORMAL  , normalDefault);

    tc = scene->GetEntity("transparentTestObject")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(5.f);
    tc->GetTransform()->SetPosition(0.0f, 5.0f, 1.0f);
    tc->GetTransform()->RotateZ(3.141572f / 2.0f);
    tc->GetTransform()->RotateX(3.141572f / 2.0f);

    component::DirectionalLightComponent* dl = scene->GetEntity("directionalLight")->GetComponent<component::DirectionalLightComponent>();
    dl->Init();
    dl->SetDirection({ -1.0f, -0.5f, -0.5f });
    

    mc = scene->GetEntity("spotLight")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(DRAW_FLAG::ForwardRendering | DRAW_FLAG::Shadow);

    tc = scene->GetEntity("spotLight")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.5f);
    tc->GetTransform()->SetPosition(-15.0f, 1.0f, 0.0f);

    component::SpotLightComponent* sl = scene->GetEntity("spotLight")->GetComponent<component::SpotLightComponent>();
    sl->SetColor(LIGHT_COLOR_TYPE::LIGHT_DIFFUSE,  { 0.8f, 0.0f, 0.0f, 1.0f });
    sl->SetDirection({ 1.0f, 0.0f, 0.0f });

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
    scene->GetEntity("cube3")->AddComponent<component::DirectionalLightComponent>(LIGHT_FLAG::CAST_SHADOW);

    mc = scene->GetEntity("cube1")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(DRAW_FLAG::ForwardRendering | DRAW_FLAG::Shadow);

    tc = scene->GetEntity("cube1")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.5f);
    tc->GetTransform()->SetPosition(-15.0f, 1.0f, 0.0f);

    mc = scene->GetEntity("cube2")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(DRAW_FLAG::ForwardRendering | DRAW_FLAG::Shadow);

    tc = scene->GetEntity("cube2")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.5f);
    tc->GetTransform()->SetPosition(-5.0f, 1.0f, 0.0f);

    mc = scene->GetEntity("cube3")->GetComponent<component::MeshComponent>();
    mc->SetMeshes(cubeModel);
    mc->SetDrawFlag(DRAW_FLAG::ForwardRendering | DRAW_FLAG::Shadow);

    tc = scene->GetEntity("cube3")->GetComponent<component::TransformComponent>();
    tc->GetTransform()->SetScale(0.5f);
    tc->GetTransform()->SetPosition(10.0f, 5.0f, 5.0f);
   
    dl = scene->GetEntity("cube3")->GetComponent<component::DirectionalLightComponent>();
    dl->Init();
    dl->SetDirection({ -1.0f, -0.5f, -0.5f });

#pragma endregion CreateScene1
    renderer.SetSceneToDraw(sceneHandler->GetScene("scene0"));
    while (!window->ExitWindow())
    {
        if (window->WasSpacePressed())
        {
            // Test to change scene during runtime
            char sceneName[10];
            static int sceneSwapper = 1;
            sceneSwapper %= 2;
            sprintf(sceneName, "scene%d", sceneSwapper);
            renderer.SetSceneToDraw(sceneHandler->GetScene(sceneName));
            sceneSwapper++;

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

        // Test to update the spotlight color
        static double red = 0.0f;
        static double green = 0.0f;
        static double blue = 0.0f;
        red += 1.1f * timer->GetDeltaTime();
        green += 0.74f * timer->GetDeltaTime();
        blue += 0.23f * timer->GetDeltaTime();
        sl->SetColor(LIGHT_COLOR_TYPE::LIGHT_DIFFUSE, { float(abs(sin(red))), float(abs(sin(green))), float(abs(sin(blue))), 1.0f });

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
