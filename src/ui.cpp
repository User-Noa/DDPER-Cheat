#include "ui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <string>

void SetupStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]       = ImVec4(0.08f, 0.08f, 0.08f, 0.98f);
    colors[ImGuiCol_Border]         = ImVec4(0.34f, 0.01f, 0.49f, 1.00f);
    colors[ImGuiCol_TitleBg]        = ImVec4(0.34f, 0.01f, 0.49f, 1.00f);
    colors[ImGuiCol_TitleBgActive]  = ImVec4(0.34f, 0.01f, 0.49f, 1.00f);
    colors[ImGuiCol_Header]         = ImVec4(0.34f, 0.01f, 0.49f, 0.70f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.34f, 0.01f, 0.49f, 0.90f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(0.34f, 0.01f, 0.49f, 1.00f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_Button]         = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.34f, 0.01f, 0.49f, 0.80f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.34f, 0.01f, 0.49f, 1.00f);
    colors[ImGuiCol_SliderGrab]     = ImVec4(0.40f, 0.01f, 0.55f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]=ImVec4(0.50f, 0.02f, 0.65f, 1.00f);
    style.WindowRounding    = 0.0f;
    style.FrameRounding     = 3.0f; 
    style.GrabRounding      = 12.0f;
    style.WindowBorderSize  = 1.0f;
    style.FrameBorderSize   = 0.0f;
}

bool CustomSliderFloat(const char* label, float* v, float v_min, float v_max) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;
    ImGuiContext& g = *GImGui; const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = ImGui::CalcItemWidth();
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + w, window->DC.CursorPos.y + label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, ImVec2(frame_bb.Max.x + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), frame_bb.Max.y));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id, &frame_bb)) return false;
    bool hovered = ImGui::ItemHoverable(frame_bb, id, 0); 
    if (hovered && ImGui::IsMouseClicked(0)) ImGui::SetActiveID(id, window);
    bool value_changed = false;
    if (g.ActiveId == id) {
        if (!ImGui::IsMouseDown(0)) ImGui::ClearActiveID();
        else {
            float grab_t = ImClamp((g.IO.MousePos.x - frame_bb.Min.x) / (frame_bb.Max.x - frame_bb.Min.x), 0.0f, 1.0f);
            float new_value = v_min + (v_max - v_min) * grab_t;
            if (*v != new_value) { *v = new_value; value_changed = true; }
        }
    }
    float track_y = frame_bb.Min.y + (frame_bb.Max.y - frame_bb.Min.y) * 0.5f;
    window->DrawList->AddLine(ImVec2(frame_bb.Min.x, track_y), ImVec2(frame_bb.Max.x, track_y), ImGui::GetColorU32(ImGuiCol_FrameBgHovered), 2.0f);
    float grab_t = ImClamp((*v - v_min) / (v_max - v_min), 0.0f, 1.0f);
    float grab_x = frame_bb.Min.x + grab_t * (frame_bb.Max.x - frame_bb.Min.x);
    window->DrawList->AddCircleFilled(ImVec2(grab_x, track_y), 6.0f, ImGui::GetColorU32(ImGuiCol_SliderGrab));
    if (label_size.x > 0.0f) ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);
    return value_changed;
}

bool CustomXCheckbox(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;
    ImGuiContext& g = *GImGui; const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const float square_sz = ImGui::GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + label_size.y + style.FramePadding.y * 2.0f));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id)) return false;
    bool hovered, held; bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) { *v = !(*v); ImGui::MarkItemEdited(id); }
    const ImRect check_bb(pos, ImVec2(pos.x + square_sz, pos.y + square_sz));
    ImGui::RenderNavHighlight(total_bb, id);
    ImGui::RenderFrame(check_bb.Min, check_bb.Max, ImGui::GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, 0.0f);
    if (*v) {
        ImU32 check_col = ImGui::GetColorU32(ImVec4(0.34f, 0.01f, 0.49f, 1.0f)); 
        float pad = ImMax(1.0f, (float)(int)(square_sz / 3.5f)); 
        window->DrawList->AddLine(ImVec2(check_bb.Min.x + pad, check_bb.Min.y + pad), ImVec2(check_bb.Max.x - pad, check_bb.Max.y - pad), check_col, 2.0f);
        window->DrawList->AddLine(ImVec2(check_bb.Max.x - pad, check_bb.Min.y + pad), ImVec2(check_bb.Min.x + pad, check_bb.Max.y - pad), check_col, 2.0f);
    }
    if (label_size.x > 0.0f) ImGui::RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), label);
    return pressed;
}

std::string KeyToStr(int key) {
    if (key < 0) return "Mouse " + std::to_string(-key);
    const char* name = SDL_GetKeyName(key);
    std::string keyName = (name && strlen(name) > 0) ? std::string(name) : "Unknown";
    std::transform(keyName.begin(), keyName.end(), keyName.begin(), ::toupper);
    return keyName;
}
