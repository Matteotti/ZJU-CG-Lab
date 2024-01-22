#include "Editor.h"

#include "Context.h"
#include "EditorSettings.h"
#include "Module/EditorModule.h"
#include "Module/Explorer.h"
#include "Module/MainMenu.h"
#include "Module/PropertyView.h"
#include "Module/SceneManager.h"
#include "Module/Viewport.h"

#include "Systems/WindowSystem.h"

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

void Editor::Init()
{
    gContext.Init();

    auto windowSystem = gContext._windowSystem;

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(windowSystem->GetWindowHandle(), true);
    ImGui_ImplOpenGL3_Init(EDITOR_GLSL_VERSION);
    ImGui::StyleColorsDark();

    auto &io = ImGui::GetIO();

    // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Load fonts
    ImFontConfig fontConfig;
    fontConfig.MergeMode = true;
    fontConfig.PixelSnapH = true;
    fontConfig.GlyphOffset = ImVec2(0.0f, 6.5f);
    static const ImWchar glyphRanges[] = {0xe000, 0xffff, 0};
    io.Fonts->AddFontFromFileTTF(EDITOR_FONT_PATH, 24.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF(EDITOR_FONT_ICON_PATH, 32.0f, &fontConfig, glyphRanges);

    // Init window
    windowSystem->RegisterOnWindowSizeFunc([](int width, int height) { gContext._reDockFlag = true; });
    windowSystem->MaximizeWindow();
    windowSystem->SetWindowTitle(EDITOR_WINDOW_TITLE);

    // Init modules
    _modules.emplace_back(std::make_shared<MainMenu>());
    _modules.emplace_back(std::make_shared<Viewport>());
    _modules.emplace_back(std::make_shared<PropertyView>());
    _modules.emplace_back(std::make_shared<SceneManager>());
    _modules.emplace_back(std::make_shared<Explorer>());

    // Enable Vsync
    windowSystem->EnableVsync();

    InitTheme();
}

void Editor::InitTheme()
{
    auto &style = ImGui::GetStyle();
    auto &colors = style.Colors;

    colors[ImGuiCol_TitleBg] = {0.113f, 0.113f, 0.113f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = {0.187f, 0.187f, 0.187f, 1.0f};

    colors[ImGuiCol_Tab] = {0.113f, 0.113f, 0.113f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = {0.113f, 0.113f, 0.113f, 1.0f};
    colors[ImGuiCol_TabActive] = {0.5f, 0.5f, 0.5f, 1.0f};
    colors[ImGuiCol_TabHovered] = {0.3f, 0.3f, 0.3f, 1.0f};

    colors[ImGuiCol_HeaderHovered] = {0.3f, 0.3f, 0.3f, 1.0f};

    colors[ImGuiCol_FrameBg] = {0.3f, 0.3f, 0.3f, 1.0f};

    style.WindowPadding = {8, 10};
    style.FramePadding = {8, 8};
    style.ItemSpacing = {8, 8};
    style.ItemInnerSpacing = {8, 8};
    style.ScrollbarSize = 16;
    style.FrameRounding = 4;
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

    if (gContext._reDockFlag)
    {
        ImGui::DockBuilderRemoveNode(dockspaceID);                            // Clear out existing layout
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace); // Add empty node
        ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

        ImGuiID dockMainID = dockspaceID;
        ImGuiID dockRightTopID = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Right, 0.35f, nullptr, &dockMainID);
        ImGuiID dockRightBottomID =
            ImGui::DockBuilderSplitNode(dockRightTopID, ImGuiDir_Down, 0.70f, nullptr, &dockRightTopID);
        ImGuiID dockBottomID = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Down, 0.30f, nullptr, &dockMainID);

        ImGui::DockBuilderDockWindow(EDITOR_MODULENAME_VIEWPORT, dockMainID);
        ImGui::DockBuilderDockWindow(EDITOR_MODULENAME_SCENEMGR, dockRightTopID);
        ImGui::DockBuilderDockWindow(EDITOR_MODULENAME_PROPVIEW, dockRightBottomID);
        ImGui::DockBuilderDockWindow(EDITOR_MODULENAME_EXPLORER, dockBottomID);

        ImGui::DockBuilderFinish(dockspaceID);

        gContext._reDockFlag = false;
    }

    ImGui::End();
}

void Editor::Run()
{
    gContext._engine.RunEx([&]() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DockSpace();
        for (auto &module : _modules)
            module->Update();

        ImGui::ShowDemoWindow();

        gContext._windowSystem->SetDeltaTimeCountMode(gContext._sceneRunning);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    });
}

void Editor::Shutdown()
{
    gContext._engine.Shutdown();
}
