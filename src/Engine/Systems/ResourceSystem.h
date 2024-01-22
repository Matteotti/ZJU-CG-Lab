#pragma once

#include "Components/Mesh.h"
#include "Components/Shader.h"
#include "Components/Texture.h"
#include "EngineSettings.h"
#include "System.h"

#include <cstddef>
#include <filesystem>
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
    friend class Explorer;

public:
    virtual void Init(bool editorMode) override;
    virtual void Shutdown() override;

    virtual void Update(float dt) override;
    virtual int GetPriority() const override;

    void LoadResource(const std::string &metaPath = ENGINE_ASSET_META_PATH);
    void LoadMesh(const std::filesystem::path &rootPath, YAML::Node &root);
    void LoadShader(const std::filesystem::path &rootPath, YAML::Node &root);
    void LoadSound(const std::filesystem::path &rootPath, YAML::Node &root);
    void LoadTexture(const std::filesystem::path &rootPath, YAML::Node &root);

    void AttachAsset(AssetType type, const std::string &resName, Entity entity);

private:
    std::unordered_map<std::string, std::size_t> _assetMap;
    std::vector<std::pair<std::string, Texture>> _textures;
    std::vector<std::pair<std::string, Shader>> _shaders;
    std::vector<std::pair<std::string, Mesh>> _meshes;
};
