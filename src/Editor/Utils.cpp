#include "Utils.h"

#include <cstdarg>
#include <cstring>

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
        ImGui::PopStyleColor(2);
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
        ImGui::PopStyleColor(2);
        return true;
    }
    ImGui::PopStyleColor(2);
    return false;
}

void CustomText(const char *label, const ImVec4 &color)
{
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
}

void CustomTextConditional(const char *label, const ImVec4 &colorFalse, const ImVec4 &colorTrue, bool condition)
{
    auto color = colorFalse;
    if (condition)
        color = colorTrue;

    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
}

char *FormatText(const char *fmt, ...)
{
    // RISK OF OVERFLOW...

    static char buffer[512];

    std::va_list args;
    va_start(args, fmt);
    std::vsprintf(buffer, fmt, args);
    va_end(args);

    return buffer;
}

void HelpMarker(const char *desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void DragDstArea()
{
    auto childFlags = ImGuiChildFlags_AutoResizeY;
    ImGui::BeginChild("DragDstArea", ImGui::GetContentRegionAvail(), childFlags);
    ImGui::EndChild();
}
