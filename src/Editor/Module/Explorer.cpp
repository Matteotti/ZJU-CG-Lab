#include "Explorer.h"

#include "EditorSettings.h"
#include "Module/EditorModule.h"

#include <memory>
#include <vector>

#include <imgui/imgui.h>

void Explorer::Update()
{
    ImGui::Begin(EDITOR_MODULENAME_EXPLORER, nullptr);
    ImGui::End();
}
