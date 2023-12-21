#pragma once

#include "Components/Mesh.h"
#include "Components/Shader.h"
#include "Components/Texture.h"
#include "System.h"

#include <string>
#include <unordered_map>
#include <vector>

#include <yaml-cpp/yaml.h>

enum class AssetType
{
    UNDEFINED,
    MESH,
    SHADER,
    SOUND,
    TEXTURE
};

class ResourceSystem : public System
{
public:
    virtual void Init() override;
    virtual void Shutdown() override;

    virtual void Update(float dt) override;
    virtual int GetPriority() const override;

    void LoadResource();
    void LoadMesh(YAML::Node &root);
    void LoadShader(YAML::Node &root);
    void LoadSound(YAML::Node &root);
    void LoadTexture(YAML::Node &root);

    void AttachAsset(AssetType type, const std::string &resName, Entity entity);

private:
    std::unordered_map<std::string, std::size_t> _assetMap;
    std::vector<Texture> _textures;
    std::vector<Shader> _shaders;
    std::vector<Mesh> _meshes;
};
