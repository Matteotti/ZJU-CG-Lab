#include "MainMenu.h"

#include "Context.h"

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
                gContext._windowSystem->CloseWindow();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("\ue3c9 Edit"))
        {
            ImGui::Text("There is nothing here :(");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("\ue8b8 Settings"))
        {
            ImGui::SeparatorText("Editor Control");
            {
                if (ImGui::Button("Reset layout"))
                {
                    gContext._reDockFlag = true;
                }
            }

            ImGui::SeparatorText("Render Settings");
            {
                static ImVec4 color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
                ImGui::ColorEdit3("Clear Color", (float *)&color);
                glClearColor(color.x, color.y, color.z, 1.0f);
            }
            {
                static bool wireframe = false;
                if (ImGui::Checkbox("Render wireframe", &wireframe))
                {
                    if (wireframe)
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    else
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }

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
