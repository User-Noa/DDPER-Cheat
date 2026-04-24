#pragma once
#include <string>
void SetupStyle();
bool CustomSliderFloat(const char* label, float* v, float v_min, float v_max);
bool CustomXCheckbox(const char* label, bool* v);
std::string KeyToStr(int key);
