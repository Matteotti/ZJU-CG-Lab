#include "MainMenu.h"

#include "Coordinator.h"
#include "Systems/WindowSystem.h"

#include <glad/glad.h>
#include <imgui/imgui.h>

void MainMenu::Update()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("\ue2c7 File"))
        {
            if (ImGui::MenuItem("Open.."))
            {
            }

            if (ImGui::MenuItem("Preferences"))
            {
            }

            if (ImGui::MenuItem("Exit"))
            {
                // send exit signal to Editor
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("\ue3c9 Edit"))
        {
            ImGui::Text("There is nothing here :(");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("\ueadb Experimental"))
        {
            ImGui::SeparatorText("Frame");
            ImGui::Text("FPS: %.0f", 1 / gCoordinator.GetSystem<WindowSystem>()->GetDeltaTime());

            ImGui::SeparatorText("Screen Settings");
            static ImVec4 color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
            ImGui::ColorEdit3("Clear Color", (float *)&color);
            glClearColor(color.x, color.y, color.z, 1.0f);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("\ue94c About"))
        {
            ImGui::SeparatorText("Website");
            ImGui::Text("https://github.com/Matteotti/ZJU-CG-Lab");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
