#include "SceneManager.h"

#include "EditorSettings.h"

#include <imgui/imgui.h>

void SceneManager::Update()
{
    ImGui::Begin(EDITOR_MODULENAME_SCENEMGR, nullptr);

    ImGui::PushStyleColor(ImGuiCol_Button, {0.0f, 0.5f, 0.0f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.0f, 0.7f, 0.0f, 1.0f});

    if (ImGui::Button("\ue145 Add Entity (NOT IMPLEMENTED YET)"))
    {
    }

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, {0.5f, 0.0f, 0.0f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.7f, 0.0f, 0.0f, 1.0f});
    if (ImGui::Button("\ue0b8 Clear"))
    {
    }

    ImGui::PopStyleColor(4);

    ImGui::End();
}
