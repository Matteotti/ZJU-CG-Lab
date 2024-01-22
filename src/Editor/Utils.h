#pragma once

#include "imgui.h"
#include <functional>

bool CustomButton(const char *label, const ImVec4 &baseColor);
bool CustomButtonConditional(const char *label, const ImVec4 &baseColorFalse, const ImVec4 &baseColorTrue,
                             bool condition);
void CustomText(const char *label, const ImVec4 &color);
void CustomTextConditional(const char *label, const ImVec4 &colorFalse, const ImVec4 &colorTrue, bool condition);
char *FormatText(const char *fmt, ...);

void DragDstArea();

void HelpMarker(const char *desc); // from imgui_demo.cpp
