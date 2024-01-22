#include "Utils.h"

#include <imgui/imgui.h>

bool CustomButton(const char *label, const ImVec4 &baseColor)
{
    auto hoverColor = baseColor;
    hoverColor.x = std::min(hoverColor.x + 0.3f, 1.0f);
    hoverColor.y = std::min(hoverColor.y + 0.3f, 1.0f);
    hoverColor.z = std::min(hoverColor.z + 0.3f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, baseColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
    if (ImGui::Button(label))
    {
        return true;
    }
    ImGui::PopStyleColor(2);
    return false;
}

bool CustomButtonConditional(const char *label, const ImVec4 &baseColorFalse, const ImVec4 &baseColorTrue,
                             bool condition)
{
    auto baseColor = baseColorFalse;
    if (condition)
        baseColor = baseColorTrue;

    auto hoverColor = baseColor;

    hoverColor.x = std::min(hoverColor.x + 0.3f, 1.0f);
    hoverColor.y = std::min(hoverColor.y + 0.3f, 1.0f);
    hoverColor.z = std::min(hoverColor.z + 0.3f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, baseColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
    if (ImGui::Button(label))
    {
        return true;
    }
    ImGui::PopStyleColor(2);
    return false;
}
