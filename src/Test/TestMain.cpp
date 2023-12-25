#include "TestHeader.h"

int main()
{
    Engine engine;

    engine.Init();

    auto resSys = gCoordinator.GetSystem<ResourceSystem>();

    std::shared_ptr<Camera> caComp = std::make_shared<Camera>();
    caComp->LookAt(glm::vec3(25.0f, 25.0f, 25.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    caComp->Perspective(100.0f, 1.0f, 0.01f, 100.0f);
    caComp->SetAsCurrentSceneCamera();

    // for (int i = 0; i < 1000; i++)
    {
        auto entity = gCoordinator.CreateEntity();

        resSys->AttachAsset(AssetType::MESH, "icosphere", entity);
        resSys->AttachAsset(AssetType::SHADER, "default", entity);
        resSys->AttachAsset(AssetType::TEXTURE, "wall", entity);

        Transform tfComp;
        tfComp.SetPosition({0, 0, 0});
        float scale = 8.0;
        tfComp.SetScale({scale, scale, scale});
        gCoordinator.AddComponent(entity, tfComp);

        Rigidbody rbComp;
        // rbComp.SetVelocity({0.0f, 0, 0});
        rbComp.SetAngularVelocity({15.0f * sin(360.0f * glfwGetTime()), 15.0f * sin(360.0f * glfwGetTime() + 120.0f),
                                   15.0f * sin(360.0f * glfwGetTime() + 120.0f)});
        gCoordinator.AddComponent(entity, rbComp);
    }

    engine.Run();
    engine.Shutdown();

    return 0;
}
