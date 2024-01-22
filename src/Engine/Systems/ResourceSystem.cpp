#include "ResourceSystem.h"

#include "Coordinator.h"
#include "Systems/LogSystem.h"
#include <cassert>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <stb/stb_image.h>

#include <filesystem>

namespace fs = std::filesystem;

void ResourceSystem::Init(bool editorMode)
{
    Super::Init(editorMode);
}

void ResourceSystem::Shutdown()
{
}

void ResourceSystem::Update(float dt)
{
    ;
}

void ResourceSystem::LoadResource(const std::string &metaPath)
{
    if (!fs::exists(metaPath))
    {
        LOG_ERROR("META file does not exist!");
        return;
    }

    fs::path rootPath(metaPath);
    rootPath = rootPath.parent_path();

    LOG_INFO("Asset Root Path: %s", rootPath.string().c_str());

    auto metaRoot = YAML::LoadFile(metaPath);
    LoadMesh(rootPath, metaRoot);
    LoadShader(rootPath, metaRoot);
    LoadTexture(rootPath, metaRoot);
}

void ResourceSystem::LoadMesh(const std::filesystem::path &rootPath, YAML::Node &root)
{
    struct Vertex
    {
        float x, y, z;
        float vx, vy, vz;
        float u, v;
    };

    auto meshNode = root["mesh"];
    auto meshPath = rootPath / ENGINE_ASSET_MESH_PATH;

    for (const auto &item : meshNode)
    {
        auto name = item.first.as<std::string>();
        auto path = meshPath / item.second.as<std::string>();

        unsigned VAO, VBO, vertexNum;

        std::ifstream fin(path);
        if (!fin)
        {
            LOG_WARNING("Could not open .obj file! (Path: %s)", path.string().c_str());
            continue;
        }

        using Dual = std::pair<float, float>;
        using Triple = std::tuple<float, float, float>;

        std::vector<Vertex> vertices;
        std::vector<Dual> vt(1);
        std::vector<Triple> v(1), vn(1);
        std::string s;

        while (1)
        {
            std::getline(fin, s);
            if (fin.fail())
                break;
            if (s[0] == 'v')
            {
                if (s[1] == ' ') // pos
                {
                    std::stringstream ssm(s.substr(2));
                    v.emplace_back();
                    auto &back = v.back();
                    ssm >> std::get<0>(back) >> std::get<1>(back) >> std::get<2>(back);
                }
                else if (s[1] == 't') // uv
                {
                    std::stringstream ssm(s.substr(2));
                    vt.emplace_back();
                    auto &back = vt.back();
                    ssm >> back.first >> back.second;
                }
                else if (s[1] == 'n') // normal
                {
                    std::stringstream ssm(s.substr(2));
                    vn.emplace_back();
                    auto &back = vn.back();
                    ssm >> std::get<0>(back) >> std::get<1>(back) >> std::get<2>(back);
                }
            }
            else if (s[0] == 'f')
            {
                std::stringstream ssm(s.substr(2));
                std::string t;
                for (int i = 0; i < 3; i++)
                {
                    ssm >> t;
                    auto slashPos1 = t.find('/');
                    auto slashPos2 = t.find('/', slashPos1 + 1);
                    auto vertexNo = std::stoi(t.substr(0, slashPos1));
                    auto uvNo = std::stoi(t.substr(slashPos1 + 1, slashPos2 - slashPos1 - 1));
                    auto normalNo = std::stoi(t.substr(slashPos2 + 1));

                    vertices.emplace_back();
                    auto &back = vertices.back();
                    back.x = std::get<0>(v[vertexNo]);
                    back.y = std::get<1>(v[vertexNo]);
                    back.z = std::get<2>(v[vertexNo]);
                    back.vx = std::get<0>(vn[normalNo]);
                    back.vy = std::get<1>(vn[normalNo]);
                    back.vz = std::get<2>(vn[normalNo]);
                    back.u = vt[uvNo].first;
                    back.v = vt[uvNo].second;
                }
            }
        }

        fin.close();

        vertexNum = vertices.size();

        // generate VBO, VAO
        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        Mesh meshComp;
        meshComp.SetVertices(VAO, VBO, vertexNum);
        _assetMap.insert({name, _meshes.size()});
        _meshes.push_back({name, meshComp});

        LOG_INFO("Loaded MESH <%s>", name.c_str());
    }
}

void ResourceSystem::LoadShader(const std::filesystem::path &rootPath, YAML::Node &root)
{
    static auto checkCompileErrors = [](GLuint id, const std::string &type) {
        static GLchar infolog[1024];

        GLint success;
        if (type != "PROGRAM")
        {
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(id, 1024, NULL, infolog);
                LOG_WARNING("%s", infolog);
            }
        }
        else
        {
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(id, 1024, NULL, infolog);
                LOG_WARNING("%s", infolog);
            }
        }
    };

    auto shaderNode = root["shader"];
    auto shaderPath = rootPath / ENGINE_ASSET_SHADER_PATH;

    for (const auto &item : shaderNode)
    {
        auto name = item.first.as<std::string>();
        auto vshaderPath = shaderPath / item.second["vs"].as<std::string>();
        auto fshaderPath = shaderPath / item.second["fs"].as<std::string>();

        std::string vshader_str;
        std::string fshader_str;
        std::ifstream vshader_fin;
        std::ifstream fshader_fin;
        vshader_fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fshader_fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vshader_fin.open(vshaderPath);
            fshader_fin.open(fshaderPath);
            std::stringstream vshader_ssm, fshader_ssm;
            vshader_ssm << vshader_fin.rdbuf();
            fshader_ssm << fshader_fin.rdbuf();
            vshader_fin.close();
            fshader_fin.close();
            vshader_str = vshader_ssm.str();
            fshader_str = fshader_ssm.str();
        }
        catch (std::ifstream::failure &e)
        {
            LOG_WARNING("Failed to load shader source: %s", e.what());
            continue;
        }

        const char *vshader_data = vshader_str.c_str();
        const char *fshader_data = fshader_str.c_str();

        unsigned int vertex, fragment, program;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vshader_data, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fshader_data, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        checkCompileErrors(program, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        Shader shaderComp;
        shaderComp.SetProgram(program);
        // INSERT FIRST THEN PUSH_BACK!! (spent 20 minutes debugging this...)
        _assetMap.insert({name, _shaders.size()});
        _shaders.push_back({name, shaderComp});

        LOG_INFO("Loaded SHADER <%s>", name.c_str());
    }
}

void ResourceSystem::LoadTexture(const std::filesystem::path &rootPath, YAML::Node &root)
{
    auto textureNode = root["texture"];
    auto texturePath = rootPath / ENGINE_ASSET_TEXTURE_PATH;

    for (const auto &item : textureNode)
    {
        auto name = item.first.as<std::string>();
        auto path = texturePath / item.second.as<std::string>();

        unsigned textureID;

        stbi_set_flip_vertically_on_load(true);

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            fs::path pathChecker(path);
            auto ext = pathChecker.extension().string();
            auto imgFormat = GL_RGBA;
            if (ext == ".jpg")
                imgFormat = GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, width, height, 0, imgFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            Texture texComp;
            texComp.SetTextureID(textureID);
            _assetMap.insert({name, _textures.size()});
            _textures.push_back({name, texComp});

            LOG_INFO("Loaded TEXTURE <%s>", name.c_str());
        }
        else
        {
            LOG_WARNING("Failed to load TEXTURE <%s> (Path: %s)", name.c_str(), path.c_str());
        }
        stbi_image_free(data);
    }
}

void ResourceSystem::AttachAsset(AssetType type, const std::string &resName, Entity entity)
{
    static std::string assetTypeStr[8]{"UNDEFINED", "MESH", "SHADER", "SOUND", "TEXTURE"};
    LOG_INFO("Attaching %s asset <%s> to entity [%d]", assetTypeStr[(int)type].c_str(), resName.c_str(), entity);
    assert(_assetMap.find(resName) != _assetMap.end());

    switch (type)
    {
    case AssetType::UNDEFINED:
        break;
    case AssetType::MESH:
        gCoordinator.AddComponent(entity, _meshes[_assetMap[resName]].second);
        break;
    case AssetType::SHADER:
        gCoordinator.AddComponent(entity, _shaders[_assetMap[resName]].second);
        break;
    case AssetType::TEXTURE:
        gCoordinator.AddComponent(entity, _textures[_assetMap[resName]].second);
        break;
    default:
        break;
    }
}

int ResourceSystem::GetPriority() const
{
    return ENGINE_PRIORITY_RESOURCE_SYSTEM;
}
