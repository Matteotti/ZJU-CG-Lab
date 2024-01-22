#include "SceneManager.h"

#include "Context.h"
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
    ImGui::Begin(EDITOR_MODULENAME_SCENEMGR);

    ImGui::PushStyleColor(ImGuiCol_Button, {0.0f, 0.5f, 0.0f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.0f, 0.7f, 0.0f, 1.0f});

    if (ImGui::Button("\ue145 Add Entity"))
    {
        OnAddEntity();
    }
    ImGui::PopStyleColor(2);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, {0.5f, 0.0f, 0.0f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.7f, 0.0f, 0.0f, 1.0f});
    if (ImGui::Button("\ue0b8 Clear ALL"))
    {
        OnClearAllEntity();
    }
    ImGui::PopStyleColor(2);

    ImGui::SameLine();
    ImGui::SeparatorText("");

    // scene info
    static char entityNameBuf[32];
    if (ImGui::TreeNodeEx("Main", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PushStyleColor(ImGuiCol_Header, {0.3f, 0.3f, 0.3f, 1.0f});
        for (auto entity : *gContext._entities)
        {
            if (gContext._selectedEntity == entity)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, {0.5f, 0.0f, 0.0f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.7f, 0.0f, 0.0f, 1.0f});
                if (ImGui::Button("\ue5cd"))
                {
                    gCoordinator.DestroyEntity(entity);

                    gContext._entities->erase(entity);
                    gContext._selectedEntity = UNDEFINED_ENTITY;
                }
                ImGui::PopStyleColor(2);

                ImGui::SameLine();
            }

            sprintf(entityNameBuf, "Entity #%d", entity);
            if (ImGui::Selectable(entityNameBuf, gContext._selectedEntity == entity))
            {
                gContext._selectedEntity = entity;
            }
        }
        ImGui::PopStyleColor();
        ImGui::TreePop();
    }

    ImGui::End();
}

void SceneManager::OnAddEntity()
{
    auto newEntity = gCoordinator.CreateEntity();
    gContext._entities->insert(newEntity);

    auto resSys = gCoordinator.GetSystem<ResourceSystem>();

    resSys->AttachAsset(AssetType::MESH, "cube", newEntity);
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
    gContext._selectedEntity = UNDEFINED_ENTITY;
    for (auto e : *gContext._entities)
    {
        gCoordinator.DestroyEntity(e);
    }
    gContext._entities->clear();
}
