#include "TestHeader.h"

int main()
{
    Engine engine;

    engine.Init();

    auto entity = gCoordinator.CreateEntity();
    auto resSys = gCoordinator.GetSystem<ResourceSystem>();
    resSys->AttachAsset(AssetType::MESH, "cube", entity);
    resSys->AttachAsset(AssetType::SHADER, "default", entity);
    resSys->AttachAsset(AssetType::TEXTURE, "wall", entity);

    engine.Run();
    engine.Shutdown();

    return 0;
}
