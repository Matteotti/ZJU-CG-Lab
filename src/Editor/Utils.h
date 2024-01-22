#pragma once

#include "imgui.h"
#include <functional>

bool CustomButton(const char *label, const ImVec4 &baseColor);
bool CustomButtonConditional(const char *label, const ImVec4 &baseColorFalse, const ImVec4 &baseColorTrue,
                             bool condition);
