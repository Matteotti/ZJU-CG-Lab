#include "SceneManager.h"

#include "EditorSettings.h"

#include "Components/Camera.h"
#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "Coordinator.h"
#include "Entity.h"
#include "Systems/RenderSystem.h"
#include "Systems/ResourceSystem.h"
#include "Systems/WindowSystem.h"

#include <cstdint>
#include <imgui/imgui.h>

void SceneManager::Update()
{
    ImGui::Begin(EDITOR_MODULENAME_SCENEMGR, nullptr);

    ImGui::PushStyleColor(ImGuiCol_Button, {0.0f, 0.5f, 0.0f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.0f, 0.7f, 0.0f, 1.0f});

    if (ImGui::Button("\ue145 Add Entity"))
    {
        OnAddEntity();
    }

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, {0.3f, 0.0f, 0.7f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.0f, 0.0f, 0.9f, 1.0f});
    if (ImGui::Button("\ue145 Add 10 Entities"))
    {
        for (int i = 0; i < 10; i++)
            OnAddEntity();
    }

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, {0.5f, 0.0f, 0.0f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.7f, 0.0f, 0.0f, 1.0f});
    if (ImGui::Button("\ue0b8 Clear"))
    {
        OnClearAllEntity();
    }

    ImGui::PopStyleColor(6);

    // scene info
    for (auto entity : *_entities)
    {
        char buf[32];
        sprintf(buf, "Entity #%d", entity);
        if (ImGui::Selectable(buf, _selectedEntity == entity))
            _selectedEntity = entity;
    }

    ImGui::End();
}

void SceneManager::OnAddEntity()
{
    auto newEntity = gCoordinator.CreateEntity();
    _entities->push_back(newEntity);

    auto resSys = gCoordinator.GetSystem<ResourceSystem>();

    resSys->AttachAsset(AssetType::MESH, "icosphere", newEntity);
    resSys->AttachAsset(AssetType::SHADER, "default", newEntity);
    resSys->AttachAsset(AssetType::TEXTURE, "wall", newEntity);

    Transform tfComp;
    tfComp.SetPosition({0, 0, 0});
    float scale = 8.0;
    tfComp.SetScale({scale, scale, scale});
    gCoordinator.AddComponent(newEntity, tfComp);

    Rigidbody rbComp;
    rbComp.SetGravity({0.0f, 0.0f, 0.0f});
    // rbComp.AddForceAtPosition({1.0f, 0.0f, 0.0f}, {0.0f, 100.0f, 0.0f}, 5.0f);
    gCoordinator.AddComponent(newEntity, rbComp);
}

void SceneManager::OnClearAllEntity()
{
    _selectedEntity = UINT32_MAX;
    for (auto e : *_entities)
    {
        gCoordinator.DestroyEntity(e);
    }
    _entities->clear();
}
