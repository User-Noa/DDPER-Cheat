// src/aimbot.h
#pragma once
#include <cstdint>

void FindAimbotTarget(float myX, float myY, int myID, uintptr_t entityList,
                      int mapW, int mapH, uintptr_t tilesBase,
                      float currentActiveFOV, bool shouldAimNow, float realAimAngle,
                      int& bestTargetID, float& bestX, float& bestY);
