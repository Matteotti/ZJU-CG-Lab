#include "PropertyView.h"

#include "Components/Shader.h"
#include "Context.h"
#include "EditorSettings.h"

#include "Components/Camera.h"
#include "Components/Rigidbody.h"
#include "Components/Texture.h"
#include "Components/Transform.h"
#include "Coordinator.h"
#include "Entity.h"
#include "Systems/RenderSystem.h"
#include "Systems/ResourceSystem.h"
#include "Utils.h"

#include <cstdint>

#include <glm/glm.hpp>
#include <imgui/imgui.h>

PropertyView::PropertyView()
{
}

void PropertyView::Update()
{
    ImGui::Begin(EDITOR_MODULENAME_PROPVIEW);

    auto selectedEntity = gContext._selectedEntity;

    if (selectedEntity != UINT32_MAX)
    {
        ViewPropEntity(selectedEntity);
        ImGui::SeparatorText("Components");
        ViewPropTransform(selectedEntity);
        ViewPropRigidbody(selectedEntity);
        ViewPropMesh(selectedEntity);
        ViewPropTexture(selectedEntity);
        ViewPropShader(selectedEntity);
    }

    ImGui::End();
}

void PropertyView::ViewPropEntity(Entity entity)
{
    gCoordinator.GetComponent<Texture>(entity);
    static Signature standardEntitySignature = gCoordinator.GetSystemSignature<RenderSystem>();

    Signature currentEntitySignature = gCoordinator.GetEntitySignature(entity);
    auto checkSignature = (standardEntitySignature & currentEntitySignature) == standardEntitySignature;

    ImGui::SeparatorText("Entity");
    {
        ImGui::Text("ID: %d", entity);
        ImGui::TextUnformatted("Status: ");
        ImGui::SameLine();
        CustomTextConditional(FormatText("%s", checkSignature ? "OK" : "INCOMPLETE"), {0.6f, 0.2f, 0.2f, 1.0f},
                              {0.2f, 0.6f, 0.2f, 1.0f}, checkSignature);

        if (!checkSignature)
        {
            ImGui::SameLine();
            HelpMarker("Add more components!");
        }
    }
}

void PropertyView::ViewPropTransform(Entity entity)
{
    auto &tfComp = gCoordinator.GetComponent<Transform>(entity);

    ImGui::PushStyleColor(ImGuiCol_Header, {0.2f, 0.2f, 0.2f, 1.0f});
    if (ImGui::CollapsingHeader("\uf71e Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        glm::vec3 pos = tfComp.GetPosition();
        glm::vec3 rotation = tfComp.GetRotation();
        glm::vec3 scale = tfComp.GetScale();

        if (ImGui::InputFloat3(" Position", &pos.x, "%.2f"))
            tfComp.SetPosition(pos);

        if (ImGui::InputFloat3(" Rotation", &rotation.x, "%.2f"))
            tfComp.SetRotation(rotation);

        if (ImGui::InputFloat3(" Scale", &scale.x, "%.2f"))
            tfComp.SetScale(scale);
    }
    ImGui::PopStyleColor();
}

void PropertyView::ViewPropRigidbody(Entity entity)
{
    auto &rbComp = gCoordinator.GetComponent<Rigidbody>(entity);

    ImGui::PushStyleColor(ImGuiCol_Header, {0.2f, 0.2f, 0.2f, 1.0f});
    if (ImGui::CollapsingHeader("\ue71c Rigidbody", ImGuiTreeNodeFlags_DefaultOpen))
    {
        glm::vec3 velocity = rbComp.GetVelocity();
        glm::vec3 angularVelocity = rbComp.GetAngularVelocity();
        glm::vec3 gravity = rbComp.GetGravity();

        if (ImGui::InputFloat3(" Velocity", &velocity.x, "%.2f"))
            rbComp.SetVelocity(velocity);

        if (ImGui::InputFloat3(" AngularVelocity", &angularVelocity.x, "%.2f"))
            rbComp.SetAngularVelocity(angularVelocity);

        if (ImGui::InputFloat3(" Gravity", &gravity.x, "%.2f"))
            rbComp.SetGravity(gravity);
    }
    ImGui::PopStyleColor();
}

void PropertyView::ViewPropTexture(Entity entity)
{
    ImGui::PushStyleColor(ImGuiCol_Header, {0.2f, 0.2f, 0.2f, 1.0f});
    if (ImGui::CollapsingHeader("\ue421 Texture", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (gCoordinator.HasComponent<Texture>(entity))
        {
            auto textureComp = gCoordinator.GetComponent<Texture>(entity);
            ImGui::ImageButton("Texture", (ImTextureID)textureComp.GetTextureID(), ImVec2(100, 100), ImVec2(0, 1),
                               ImVec2(1, 0));
        }
        else
        {
            CustomText("(Drag assets from Explorer to me..)", {0.4f, 0.4f, 0.4f, 1.0f});
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("TEXTURE"))
                {
                    auto name = (const char *)payload->Data;
                    gContext._resourceSystem->AttachAsset(AssetType::TEXTURE, name, entity);
                }
                ImGui::EndDragDropTarget();
            }
        }
    }
    ImGui::PopStyleColor();
}

void PropertyView::ViewPropMesh(Entity entity)
{
    Mesh meshComp;

    ImGui::PushStyleColor(ImGuiCol_Header, {0.2f, 0.2f, 0.2f, 1.0f});
    if (ImGui::CollapsingHeader("\uf720 Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (gCoordinator.HasComponent<Mesh>(entity))
        {
            ImGui::Text("Mesh applied!");
        }
        else
        {
            CustomText("(Drag assets from Explorer to me..)", {0.4f, 0.4f, 0.4f, 1.0f});
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("MESH"))
                {
                    auto name = (const char *)payload->Data;
                    gContext._resourceSystem->AttachAsset(AssetType::MESH, name, entity);
                }
                ImGui::EndDragDropTarget();
            }
        }
    }
    ImGui::PopStyleColor();
}

void PropertyView::ViewPropShader(Entity entity)
{
    Shader shaderComp;

    ImGui::PushStyleColor(ImGuiCol_Header, {0.2f, 0.2f, 0.2f, 1.0f});
    if (ImGui::CollapsingHeader("\ue574 Shader", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (gCoordinator.HasComponent<Shader>(entity))
        {
            ImGui::Text("Shader applied!");
        }
        else
        {
            CustomText("(Drag assets from Explorer to me..)", {0.4f, 0.4f, 0.4f, 1.0f});
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("SHADER"))
                {
                    auto name = (const char *)payload->Data;
                    gContext._resourceSystem->AttachAsset(AssetType::SHADER, name, entity);
                }
                ImGui::EndDragDropTarget();
            }
        }
    }
    ImGui::PopStyleColor();
}
