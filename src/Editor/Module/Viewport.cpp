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

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 64);
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 10);

        ImGui::PushStyleColor(ImGuiCol_Button, {0.2f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.5f, 0.5f, 0.5f, 1.0f});
        ImGui::Button("\ue037");

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, {0.2f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.5f, 0.5f, 0.5f, 1.0f});
        ImGui::Button("\ue034");

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, {0.2f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.5f, 0.5f, 0.5f, 1.0f});
        ImGui::Button("\ue042");
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(6);

        ImGui::EndTabBar();
    }

    ImGui::End();
}
