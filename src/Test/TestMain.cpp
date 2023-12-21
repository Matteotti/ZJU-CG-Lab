#include "TestHeader.h"

int main()
{
    Engine engine;

    engine.Init();

    auto resSys = gCoordinator.GetSystem<ResourceSystem>();
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
    }

    engine.Run();
    engine.Shutdown();

    return 0;
}
