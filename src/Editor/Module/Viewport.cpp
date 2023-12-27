#include "Viewport.h"

#include "EditorSettings.h"

#include "Coordinator.h"
#include "Systems/RenderSystem.h"

#include <imgui/imgui.h>

void Viewport::Update()
{
    ImGui::Begin(EDITOR_MODULENAME_VIEWPORT, nullptr);

    if (ImGui::BeginTabBar("Viewport Tab Bar"))
    {
        if (ImGui::BeginTabItem("Main Viewport"))
        {
            auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
            auto width = ImGui::GetWindowWidth(), height = ImGui::GetWindowHeight() - 2.8f * ImGui::GetItemRectSize().y;
            auto textureID = renderSystem->GetPostProcFramebufferTexture();

            renderSystem->GetCurrentCamera()->Perspective(90.0f, width / height, 0.1f, 100.0f);

            ImGui::Image((ImTextureID)textureID, ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("\ue145 New Viewport"))
        {
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}
