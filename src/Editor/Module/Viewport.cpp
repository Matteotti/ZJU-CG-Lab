#include "Viewport.h"

#include "Context.h"
#include "EditorSettings.h"
#include "Utils.h"

#include "Coordinator.h"
#include "Systems/RenderSystem.h"
#include "Systems/WindowSystem.h"

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

    ShowOverlay();

    ImGui::End();
}

void Viewport::ShowOverlay()
{
    auto viewportWindowPos = ImGui::GetWindowPos();
    auto viewportWindowSize = ImGui::GetWindowSize();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    ImGui::SetNextWindowBgAlpha(0.3f);

    ImGui::Begin("Viewport Overlay", nullptr, windowFlags);
    ImGui::SetWindowPos({viewportWindowPos.x + viewportWindowSize.x - ImGui::GetWindowSize().x, viewportWindowPos.y});

    // FPS display
    {
        static float fpsArr[128];
        static int fpsArrWriteIdx = 0;
        auto currentFPS = std::floor(1.0f / gContext._windowSystem->GetDeltaTime());
        fpsArr[fpsArrWriteIdx] = currentFPS;

        if (fpsArrWriteIdx == 128)
            fpsArrWriteIdx = 0;
        else
            ++fpsArrWriteIdx;

        if (gContext._sceneRunning)
            ImGui::Text("FPS: %.0f", currentFPS);
        else
            ImGui::Text("FPS: N/A");

        ImGui::PlotLines("", fpsArr, IM_ARRAYSIZE(fpsArr));
    }

    // Scene Control
    ImGui::SeparatorText("Scene Control");
    {
        if (CustomButtonConditional("\ue037", {0.2f, 0.6f, 0.2f, 1.0f}, {0.2f, 0.2f, 0.2f, 1.0f},
                                    gContext._sceneRunning))
        {
            gContext._sceneRunning = true;
        }

        ImGui::SameLine();
        if (CustomButtonConditional("\ue034", {0.2f, 0.2f, 0.2f, 1.0f}, {0.6f, 0.2f, 0.2f, 1.0f},
                                    gContext._sceneRunning))
        {
            gContext._sceneRunning = false;
        }
    }

    ImGui::End();
}
