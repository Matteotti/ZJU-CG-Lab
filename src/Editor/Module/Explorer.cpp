#include "Explorer.h"

#include "Context.h"
#include "EditorSettings.h"
#include "Module/EditorModule.h"
#include "Utils.h"

#include "Systems/RenderSystem.h"
#include "Systems/ResourceSystem.h"

#include <Windows.h>
#include <commdlg.h>
#include <cstddef>
#include <memory>
#include <vector>

#include <imgui/imgui.h>

void Explorer::Update()
{
    ImGui::Begin(EDITOR_MODULENAME_EXPLORER, nullptr,
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        auto contentSize = ImGui::GetContentRegionAvail();
        auto childFlags = ImGuiChildFlags_AutoResizeY;

        enum class Category
        {
            UNDEFINED,
            MESH,
            SHADER,
            TEXTURE
        };
        static Category selected = Category::UNDEFINED;

        {
            static std::string filePath("test");

            if (!gContext._resLoaded)
            {
                if (CustomButton("\ue145 LOAD", {0.2f, 0.5f, 0.2f, 1.0f}))
                {
                    if (DiagOpenFile(filePath, ""))
                    {
                        gContext._resourceSystem->LoadResource(filePath);
                        gContext._resLoaded = true;
                    }
                }
                ImGui::SetItemTooltip("Load an existing META.yml");
                ImGui::SameLine();
            }

            if (CustomButton("\ue6b8 EXPORT", {0.6f, 0.2f, 0.6f, 1.0f}))
            {
                if (DiagSaveFile(filePath, "META File (META.yml)"))
                {
                }
            }
            ImGui::SetItemTooltip("Export to a META.yml");

            ImGui::SameLine();

            ImGui::SeparatorText("");
        }

        ImGui::BeginChild("Directory Explorer", ImVec2(contentSize.x / 4, contentSize.y), childFlags);
        {
            auto meshNum = gContext._resourceSystem->_meshes.size();
            auto shaderNum = gContext._resourceSystem->_shaders.size();
            auto textureNum = gContext._resourceSystem->_textures.size();

            ImGui::PushStyleColor(ImGuiCol_Header, {0.3f, 0.3f, 0.3f, 1.0f});
            ImGui::SeparatorText("Category");
            ImGui::Indent(20);
            if (ImGui::Selectable(FormatText("\uf720 Mesh (%u)", meshNum), selected == Category::MESH))
            {
                selected = Category::MESH;
            }
            if (ImGui::Selectable(FormatText("\ue574 Shader (%u)", shaderNum), selected == Category::SHADER))
            {
                selected = Category::SHADER;
            }
            if (ImGui::Selectable(FormatText("\ue421 Texture (%u)", textureNum), selected == Category::TEXTURE))
            {
                selected = Category::TEXTURE;
            }
            ImGui::Unindent();
            ImGui::SeparatorText("All");
            ImGui::PopStyleColor();

            ImGui::EndChild();
        }

        ImGui::SameLine();

        ImGui::BeginChild("Directory Details", ImVec2(-FLT_MIN, contentSize.y), childFlags);
        {
            auto &meshes = gContext._resourceSystem->_meshes;
            auto &shaders = gContext._resourceSystem->_shaders;
            auto &textures = gContext._resourceSystem->_textures;

            ImGuiStyle &style = ImGui::GetStyle();
            int buttons_count = 20;
            float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

            switch (selected)
            {
            case Category::MESH:

                for (std::size_t i = 0; i < meshes.size(); i++)
                {
                    ImGui::PushID(i);

                    ImGui::Button(meshes[i].first.c_str(), {100.0f, 100.0f});
                    float last_button_x2 = ImGui::GetItemRectMax().x;
                    float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 100.0f;
                    if (i + 1 < meshes.size() && next_button_x2 < window_visible_x2)
                        ImGui::SameLine();

                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                    {
                        ImGui::SetDragDropPayload("MESH", meshes[i].first.c_str(), sizeof(void *));
                        ImGui::Text(meshes[i].first.c_str());
                        ImGui::EndDragDropSource();
                    }

                    ImGui::PopID();
                }
                break;

            case Category::SHADER:
                for (std::size_t i = 0; i < shaders.size(); i++)
                {
                    ImGui::PushID(i);
                    ImGui::Button(shaders[i].first.c_str(), {100.0f, 100.0f});

                    float last_button_x2 = ImGui::GetItemRectMax().x;
                    float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 100.0f;
                    if (i + 1 < shaders.size() && next_button_x2 < window_visible_x2)
                        ImGui::SameLine();

                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                    {
                        ImGui::SetDragDropPayload("SHADER", shaders[i].first.c_str(), sizeof(void *));
                        ImGui::Text(shaders[i].first.c_str());
                        ImGui::EndDragDropSource();
                    }
                    ImGui::PopID();
                }
                break;

            case Category::TEXTURE:
                for (std::size_t i = 0; i < textures.size(); i++)
                {
                    ImGui::PushID(i);
                    ImGui::ImageButton(FormatText("%s", textures[i].first.c_str()),
                                       (ImTextureID)textures[i].second.GetTextureID(), ImVec2(100, 100), ImVec2(0, 1),
                                       ImVec2(1, 0));

                    float last_button_x2 = ImGui::GetItemRectMax().x;
                    float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 100.0f;
                    if (i + 1 < textures.size() && next_button_x2 < window_visible_x2)
                        ImGui::SameLine();

                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                    {
                        ImGui::SetDragDropPayload("TEXTURE", textures[i].first.c_str(), sizeof(void *));
                        ImGui::Text(textures[i].first.c_str());
                        ImGui::EndDragDropSource();
                    }
                    ImGui::PopID();
                }
                break;

            default:
                break;
            }
            ImGui::EndChild();
        }
    }
    ImGui::End();
}

bool Explorer::DiagOpenFile(std::string &filePath, const std::string &filter)
{
    static char buffer[512];

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = filter.c_str();
    ofn.Flags = OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    auto result = GetOpenFileName(&ofn);
    if (result)
        filePath = buffer;
    return result;
}

bool Explorer::DiagSaveFile(std::string &filePath, const std::string &filter)
{
    static char buffer[512];

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = filter.c_str();
    ofn.Flags = OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    auto result = GetSaveFileName(&ofn);
    if (result)
        filePath = buffer;
    return result;
}
