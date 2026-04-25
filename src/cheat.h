#pragma once
#include <iostream>
#include <thread>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <dlfcn.h>
#include <cmath>
#include <cstdint>
#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <vector>
#include <mutex>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "offsets.h"

// ---------- متغیرهای گلوبال ----------
extern bool menuOpen;
extern bool aimbotEnabled;
extern bool espEnabled;
extern uintptr_t moduleBase;
extern int aimbotMode;
extern bool edgeScanEnabled;
extern float edgeScanRadius;
extern float edgeScanPoints;
extern bool showClassicFOV;
extern bool showLaserFOV;
extern bool showHookFOV;
extern bool showConeFOV;
extern float classicFOV;
extern float laserFOV;
extern float hookFOV;
extern float espDangerDistance;
extern float afkTimeoutMs;
extern bool fngMode;
extern bool triggerBotEnabled;
extern uint32_t lastShootTime;
extern uint32_t lastSwitchTime;
extern int prevWeaponID;
extern bool needsWeaponSwitchBack;
extern bool coneEnabled;
extern float coneAngle;
extern float coneLength;
extern bool spinBotEnabled;
extern float spinBotSpeed;
extern float currentSpinAngle;
extern float espDefaultColor[3];
extern float espDangerColor[3];
extern float coneColor[3];
extern float classicFovColor[3];
extern float hookFovColor[3];
extern float laserFovColor[3];
extern int aimHotkey;
extern int espHotkey;
extern int waitingForKey;
extern float mouseX, mouseY;
extern bool leftMouseDown, rightMouseDown;
extern bool visCheckEnabled; //visoption

struct PlayerState {
    float lastX = 0; float lastY = 0; uint32_t lastMoveTime = 0;
};
extern PlayerState players[128];
extern std::mutex eventMutex;
extern std::vector<SDL_Event> eventQueue;

extern bool imguiInitDone;
extern float predictionFactor;
