#include "PropertyView.h"

#include "EditorSettings.h"

#include "Components/Camera.h"
#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "Coordinator.h"
#include "Systems/RenderSystem.h"
#include "Systems/ResourceSystem.h"
#include "Systems/WindowSystem.h"

#include <glfw/glfw3.h>
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
    gCoordinator.AddComponent(_testEntity, rbComp);
}

void PropertyView::Update()
{
    ImGui::Begin(EDITOR_MODULENAME_PROPVIEW, nullptr);

    ImGui::SeparatorText("Transform");

    auto &tfComp = gCoordinator.GetComponent<Transform>(_testEntity);

    static glm::vec3 pos;
    if (ImGui::InputFloat3(" Position", &pos.x, "%.2f"))
    {
        tfComp.SetPosition(pos);
    }

    static glm::vec3 rotation;
    if (ImGui::InputFloat3(" Rotation", &rotation.x, "%.2f"))
    {
        tfComp.SetRotation(rotation);
    }

    static glm::vec3 scale(8.0f, 8.0f, 8.0f);
    if (ImGui::InputFloat3(" Scale", &scale.x, "%.2f"))
    {
        tfComp.SetScale(scale);
    }

    ImGui::SeparatorText("Rigidbody");

    auto &rbComp = gCoordinator.GetComponent<Rigidbody>(_testEntity);

    static glm::vec3 velocity;
    if (ImGui::InputFloat3(" Velocity", &velocity.x, "%.2f"))
    {
        rbComp.SetVelocity(velocity);
    }

    static glm::vec3 angularVelocity;
    if (ImGui::InputFloat3(" AngularVelocity", &angularVelocity.x, "%.2f"))
    {
        rbComp.SetAngularVelocity(angularVelocity);
    }

    ImGui::End();
}
