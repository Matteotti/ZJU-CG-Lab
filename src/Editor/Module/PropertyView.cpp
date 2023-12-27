#include "PropertyView.h"

#include "EditorSettings.h"

#include "Components/Camera.h"
#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "Coordinator.h"
#include "Systems/RenderSystem.h"
#include "Systems/ResourceSystem.h"
#include "Systems/WindowSystem.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

PropertyView::PropertyView()
{
    // ONLY FOR TEST
    auto resSys = gCoordinator.GetSystem<ResourceSystem>();

    std::shared_ptr<Camera> caComp = std::make_shared<Camera>();
    caComp->SetAsCurrentSceneCamera();

    auto camEntity = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(camEntity, *caComp);

    Transform tfCamComp;
    tfCamComp.SetPosition({0.0f, 0.0f, 30.0f});
    tfCamComp.SetRotation({10.0f, 0.0f, 0.0f});
    gCoordinator.AddComponent(camEntity, tfCamComp);

    _testEntity = gCoordinator.CreateEntity();

    resSys->AttachAsset(AssetType::MESH, "menger", _testEntity);
    resSys->AttachAsset(AssetType::SHADER, "default", _testEntity);
    resSys->AttachAsset(AssetType::TEXTURE, "wall", _testEntity);

    Transform tfComp;
    tfComp.SetPosition({0, 0, 0});
    float scale = 8.0;
    tfComp.SetScale({scale, scale, scale});
    gCoordinator.AddComponent(_testEntity, tfComp);

    Rigidbody rbComp;
    rbComp.SetGravity({0.0f, 0.0f, 0.0f});
    // rbComp.AddForceAtPosition({1.0f, 0.0f, 0.0f}, {0.0f, 100.0f, 0.0f}, 5.0f);
    gCoordinator.AddComponent(_testEntity, rbComp);
}

void PropertyView::Update()
{
    ImGui::Begin(EDITOR_MODULENAME_PROPVIEW);

    ViewProp(_testEntity);
    ViewProp(gCoordinator.GetComponent<Transform>(_testEntity));
    ViewProp(gCoordinator.GetComponent<Rigidbody>(_testEntity));

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
