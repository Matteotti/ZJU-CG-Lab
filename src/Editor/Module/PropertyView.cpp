#include "PropertyView.h"

#include "Components/Texture.h"
#include "EditorSettings.h"

#include "Components/Camera.h"
#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "Coordinator.h"

#include <cstdint>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

PropertyView::PropertyView()
{
}

void PropertyView::Update()
{
    ImGui::Begin(EDITOR_MODULENAME_PROPVIEW);

    if (_selectedEntity != UINT32_MAX)
    {
        ViewProp(_selectedEntity);
        ViewProp(gCoordinator.GetComponent<Transform>(_selectedEntity));
        ViewProp(gCoordinator.GetComponent<Rigidbody>(_selectedEntity));
        ViewProp(gCoordinator.GetComponent<Texture>(_selectedEntity));
    }

    ImGui::End();
}

void PropertyView::ViewProp(Entity entity)
{
    ImGui::PushStyleColor(ImGuiCol_Header, {0.2f, 0.2f, 0.2f, 1.0f});
    if (ImGui::CollapsingHeader("\ue88e Entity Info", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("ID: %d", entity);
    }
    ImGui::PopStyleColor();
}

void PropertyView::ViewProp(Transform &tfComp)
{
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

void PropertyView::ViewProp(Rigidbody &rbComp)
{
    ImGui::PushStyleColor(ImGuiCol_Header, {0.2f, 0.2f, 0.2f, 1.0f});
    if (ImGui::CollapsingHeader("\uf720 Rigidbody", ImGuiTreeNodeFlags_DefaultOpen))
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

void PropertyView::ViewProp(Texture &rbComp)
{
    ImGui::PushStyleColor(ImGuiCol_Header, {0.2f, 0.2f, 0.2f, 1.0f});
    if (ImGui::CollapsingHeader("\ue421 Texture", ImGuiTreeNodeFlags_DefaultOpen))
    {
    }
    ImGui::PopStyleColor();
}
