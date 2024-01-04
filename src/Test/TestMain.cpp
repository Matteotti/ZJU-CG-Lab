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
    tfCamComp.SetPosition({0.0f, 0.0f, 15.0f});
    tfCamComp.SetRotation({0.0f, 0.0f, 0.0f});
    gCoordinator.AddComponent(camEntity, tfCamComp);

    auto other = gCoordinator.CreateEntity();
    auto entity = gCoordinator.CreateEntity();

    // for (int i = 0; i < 1000; i++)
    {

        resSys->AttachAsset(AssetType::MESH, "cube", entity);
        resSys->AttachAsset(AssetType::SHADER, "default", entity);
        resSys->AttachAsset(AssetType::TEXTURE, "wall", entity);

        Transform tfComp;
        tfComp.SetPosition({0.2f, 5.0f, 0});
        tfComp.SetRotation({-35, 0, -45});
        float scale = 1.0;
        tfComp.SetScale({scale, scale, scale});
        gCoordinator.AddComponent(entity, tfComp);

        Rigidbody rbComp;
        rbComp.SetGravity({0.0f, 0.0f, 0.0f});
        rbComp.SetVelocity({0.0f, -1.0f, 0.0f});
        gCoordinator.AddComponent(entity, rbComp);

        Collider colComp;
        colComp.SetBoxHalfSize({1.0f, 1.0f, 1.0f});
        gCoordinator.AddComponent(entity, colComp);
    }

    {

        resSys->AttachAsset(AssetType::MESH, "cube", other);
        resSys->AttachAsset(AssetType::SHADER, "default", other);
        resSys->AttachAsset(AssetType::TEXTURE, "wall", other);

        Transform otherTfComp;
        otherTfComp.SetPosition({0, -3, 0});
        float scale = 1.0;
        otherTfComp.SetScale({scale, scale, scale});
        gCoordinator.AddComponent(other, otherTfComp);

        Rigidbody otherRbComp;
        otherRbComp.SetGravity({0.0f, 0.0f, 0.0f});
        gCoordinator.AddComponent(other, otherRbComp);

        Collider otherColComp;
        otherColComp.SetBoxHalfSize({1.0f, 1.0f, 1.0f});
        gCoordinator.AddComponent(other, otherColComp);
    }

    engine.Run();
    engine.Shutdown();

    return 0;
}