#include "Editor.h"

#include "EditorSettings.h"
#include "Module/Explorer.h"
#include "Module/MainMenu.h"
#include "Module/PropertyView.h"
#include "Module/SceneManager.h"
#include "Module/Viewport.h"

#include "Coordinator.h"
#include "Systems/RenderSystem.h"
#include "Systems/WindowSystem.h"

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>

void Editor::Init()
{
    // Init engine & get system handles
    _engine.Init(true);
    _renderSystem = gCoordinator.GetSystem<RenderSystem>();
    _windowSystem = gCoordinator.GetSystem<WindowSystem>();

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(gCoordinator.GetSystem<WindowSystem>()->GetWindowHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui::StyleColorsDark();

    auto &io = ImGui::GetIO();

    // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Load fonts
    ImFontConfig fontConfig;
    fontConfig.MergeMode = true;
    fontConfig.PixelSnapH = true;
    fontConfig.GlyphOffset = ImVec2(0.0f, 5.0f);
    ImWchar glyphRanges[] = {0xe000, 0xf200, 0};
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 24.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF(EDITOR_FONT_ICON, 24.0f, &fontConfig, glyphRanges);

    // Init window
    auto window = _windowSystem->GetWindowHandle();
    glfwMaximizeWindow(window);
    glfwGetFramebufferSize(window, &_fbWidth, &_fbHeight);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GL_FALSE);
    glfwSetWindowTitle(window, EDITOR_WINDOW_TITLE);

    // Init modules
    _modules.emplace_back(std::make_shared<MainMenu>());
    _modules.emplace_back(std::make_shared<Viewport>());
    _modules.emplace_back(std::make_shared<PropertyView>());
    _modules.emplace_back(std::make_shared<SceneManager>());
    _modules.emplace_back(std::make_shared<Explorer>());

    // Test
    glfwSwapInterval(1);
}

void Editor::DockSpace()
{
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    windowFlags |=
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("_DockSpace", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspaceID = ImGui::GetID("Dockspace");
    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);

    if (_reDockFlag)
    {
        ImGui::DockBuilderRemoveNode(dockspaceID);                            // Clear out existing layout
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace); // Add empty node
        ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

        ImGuiID dockMainID = dockspaceID;
        ImGuiID dockRightTopID = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Right, 0.30f, nullptr, &dockMainID);
        ImGuiID dockRightBottomID =
            ImGui::DockBuilderSplitNode(dockRightTopID, ImGuiDir_Down, 0.70f, nullptr, &dockRightTopID);
        ImGuiID dockBottomID = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Down, 0.30f, nullptr, &dockMainID);

        ImGui::DockBuilderDockWindow(EDITOR_MODULENAME_VIEWPORT, dockMainID);
        ImGui::DockBuilderDockWindow(EDITOR_MODULENAME_SCENEMGR, dockRightTopID);
        ImGui::DockBuilderDockWindow(EDITOR_MODULENAME_PROPVIEW, dockRightBottomID);
        ImGui::DockBuilderDockWindow(EDITOR_MODULENAME_EXPLORER, dockBottomID);

        ImGui::DockBuilderFinish(dockspaceID);

        _reDockFlag = false;
    }

    ImGui::End();
}

void Editor::Run()
{
    _engine.RunEx([&]() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DockSpace();
        for (auto &module : _modules)
            module->Update();

        // ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    });
}
