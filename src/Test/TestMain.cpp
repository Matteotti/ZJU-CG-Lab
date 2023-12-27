#include "TestHeader.h"

int main()
{
    Engine engine;

    engine.Init();

    auto resSys = gCoordinator.GetSystem<ResourceSystem>();

    std::shared_ptr<Camera> caComp = std::make_shared<Camera>();
    caComp->Perspective(60.0f, 1.0f, 0.01f, 10000.0f);
    caComp->SetAsCurrentSceneCamera();

    auto camEntity = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(camEntity, *caComp);

    Transform tfCamComp;
    tfCamComp.SetPosition({0.0f, 0.0f, 75.0f});
    tfCamComp.SetRotation({0.0f, 0.0f, 0.0f});
    gCoordinator.AddComponent(camEntity, tfCamComp);

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
        rbComp.SetGravity({0.0f, 0.0f, 0.0f});
        rbComp.AddForceAtPosition({1.0f, 0.0f, 0.0f}, {0.0f, 100.0f, 0.0f}, 5.0f);
        gCoordinator.AddComponent(entity, rbComp);
    }

    engine.Run();
    engine.Shutdown();

    return 0;
}