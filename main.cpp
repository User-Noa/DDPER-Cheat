#include "cheat.h"
#include "memory.h"
#include "render.h"
#include "ui.h"
#include "hooks.h"
#include "aimbot.h"
#include "esp.h"
#include "triggerbot.h"
#include "spinbot.h"

// تعریف کلیه متغیرهای گلوبال
bool menuOpen = false;
bool aimbotEnabled = false;
bool espEnabled = false;
uintptr_t moduleBase = 0;
int aimbotMode = 0;
bool edgeScanEnabled = false;
float edgeScanRadius = 14.0f;
float edgeScanPoints = 8.0f;
bool showClassicFOV = false;
bool showLaserFOV = false;
bool showHookFOV = false;
bool showConeFOV = false;
float classicFOV = 450.0f;
float laserFOV = 800.0f;
float hookFOV = 380.0f;
float espDangerDistance = 900.0f;
float afkTimeoutMs = 2000.0f;
bool fngMode = false;
bool triggerBotEnabled = false;
uint32_t lastShootTime = 0;
uint32_t lastSwitchTime = 0;
int prevWeaponID = -1;
bool needsWeaponSwitchBack = false;
bool visCheckEnabled = true;   // Visibility Check روشن باشد (پیش‌فرض)
bool coneEnabled = false;
float coneAngle = 60.0f;
float coneLength = 300.0f;
bool spinBotEnabled = false;
float spinBotSpeed = 15.0f;
float currentSpinAngle = 0.0f;
float espDefaultColor[3] = {0.34f, 0.01f, 0.49f};
float espDangerColor[3] = {1.0f, 0.0f, 0.0f};
float coneColor[3] = {0.0f, 0.0f, 0.0f};
float classicFovColor[3] = {0.34f, 0.01f, 0.49f};
float hookFovColor[3] = {0.2f, 0.6f, 0.8f};
float laserFovColor[3] = {0.8f, 0.2f, 0.2f};
int aimHotkey = SDLK_f;
int espHotkey = SDLK_v;
int waitingForKey = 0;
float mouseX = 0, mouseY = 0;
bool leftMouseDown = false, rightMouseDown = false;
PlayerState players[128];
std::mutex eventMutex;
std::vector<SDL_Event> eventQueue;
bool imguiInitDone = false;
float predictionFactor = 0.05f;

typedef void (*SDL_GL_SwapWindow_t)(SDL_Window* window);
SDL_GL_SwapWindow_t o_SDL_GL_SwapWindow = nullptr;

extern "C" void SDL_GL_SwapWindow(SDL_Window* window) {
    if (!o_SDL_GL_SwapWindow) {
        o_SDL_GL_SwapWindow = (SDL_GL_SwapWindow_t)dlsym(RTLD_NEXT, "SDL_GL_SwapWindow");
        moduleBase = GetModuleBase("DDPER", 2);
    }

    if (menuOpen) SDL_SetRelativeMouseMode(SDL_FALSE);

    if (!glInitDone) InitModernGL();

    if (glInitDone && !imguiInitDone) {
        IMGUI_CHECKVERSION(); ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig cfg; cfg.OversampleH = 3; cfg.OversampleV = 3;
        io.Fonts->AddFontFromFileTTF("/usr/share/fonts/noto/NotoSans-Bold.ttf", 18.0f, &cfg);
        if (io.Fonts->Fonts.empty()) io.Fonts->AddFontDefault(&cfg); 
        SetupStyle();
        ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
        ImGui_ImplOpenGL3_Init("#version 330 core");
        imguiInitDone = true;
    }

    int screenW, screenH;
    SDL_GetWindowSize(window, &screenW, &screenH);

    float currentActiveFOV = 0.0f;
    bool shouldAimNow = false;

    if (aimbotMode == 0) {
        currentActiveFOV = classicFOV; shouldAimNow = aimbotEnabled;
    } else if (aimbotMode == 1) {
        if (leftMouseDown) { currentActiveFOV = laserFOV; shouldAimNow = aimbotEnabled; }
        else if (rightMouseDown) { currentActiveFOV = hookFOV; shouldAimNow = aimbotEnabled; }
    }

    if (moduleBase) {
        int myID = -1;
        uintptr_t id_base = ReadMem<uintptr_t>(moduleBase + 0x1A88);
        if (id_base) {
            uintptr_t id_off = ReadMem<uintptr_t>(id_base + 0x1734);
            if (id_off) myID = ReadMem<int>(id_off + 0x6808);
        }

        uintptr_t ent_base = ReadMem<uintptr_t>(moduleBase + 0x1AB0);
        uintptr_t entityList = 0;
        if (ent_base) {
            uintptr_t ent_off = ReadMem<uintptr_t>(ent_base + 0xAD2C);
            if (ent_off) entityList = ReadMem<uintptr_t>(ent_off + 0xD7C8);
        }

        uintptr_t aimBaseAddr = ReadMem<uintptr_t>(moduleBase + 0x1AA0);

        int mapW = 0, mapH = 0;
        uintptr_t tilesBase = 0;
        uintptr_t p1 = ReadMem<uintptr_t>(moduleBase + 0x1A40);
        uintptr_t p2 = p1 ? ReadMem<uintptr_t>(p1 + 0xD98) : 0;
        uintptr_t mapBase = p2 ? ReadMem<uintptr_t>(p2 + 0xA30) : 0;
        if (mapBase) {
            mapW = ReadMem<int>(mapBase + 0x60);
            mapH = ReadMem<int>(mapBase + 0x64);
            tilesBase = ReadMem<uintptr_t>(mapBase + 0x68);
        }

        if (myID >= 0 && entityList && aimBaseAddr) {
            uintptr_t localPlayerBlock = entityList + (uintptr_t)myID * ENTITY_SIZE;
            float myX = ReadMem<float>(localPlayerBlock + OFF_POS_X);
            float myY = ReadMem<float>(localPlayerBlock + OFF_POS_Y);
            int currentWeapon = ReadMem<int>(localPlayerBlock + OFF_WEAPON);

            float currentAimX = ReadMem<float>(aimBaseAddr + 0x10);
            float currentAimY = ReadMem<float>(aimBaseAddr + 0x14);
            float realAimAngle = atan2(currentAimY, currentAimX);

            uint32_t currentTime = SDL_GetTicks();

            // ESP (رسم خطوط)
            DrawESP(myX, myY, myID, entityList, screenW, screenH, currentTime);

            // Aimbot: یافتن بهترین هدف
            int bestTargetID = -1;
            float bestX = 0, bestY = 0;
            FindAimbotTarget(myX, myY, myID, entityList, mapW, mapH, tilesBase,
                             currentActiveFOV, shouldAimNow, realAimAngle,
                             bestTargetID, bestX, bestY);

            bool targetAquired = (bestTargetID != -1) && (shouldAimNow || (fngMode && triggerBotEnabled));

            // اعمال هدف یا اسپین‌بات
            if (targetAquired) {
                WriteMem<float>(aimBaseAddr + 0x10, bestX - myX);
                WriteMem<float>(aimBaseAddr + 0x14, bestY - myY);

                HandleTriggerBot(currentTime, true, currentWeapon);
            } else {
                HandleSpinBot(aimBaseAddr, spinBotEnabled);
            }

            // رسم پوشش‌های FOV
            DrawFOVOverlays(screenW, screenH, realAimAngle, aimbotMode);
        }
    }

    // منوی ImGui
    if (imguiInitDone) {
        {
            std::lock_guard<std::mutex> lock(eventMutex);
            for (const auto& ev : eventQueue) {
                ImGui_ImplSDL2_ProcessEvent(&ev);
            }
            eventQueue.clear();
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (menuOpen) {
            ImGui::SetNextWindowSize(ImVec2(530, 850), ImGuiCond_Appearing); 
            ImGui::Begin("DDPER CHEAT | BY Noa", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

            ImGuiColorEditFlags clrFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoAlpha;

            CustomXCheckbox("Aimbot", &aimbotEnabled);
            ImGui::SameLine(180);
            if (waitingForKey == 1) {
                ImGui::Text("[...]");
            } else {
                std::string aimTxt = "[" + KeyToStr(aimHotkey) + "]";
                ImGui::Text("%s", aimTxt.c_str());
                if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::IsItemClicked()) waitingForKey = 1;
            }

            CustomXCheckbox("ESP Wallhack", &espEnabled);
            ImGui::SameLine(180);
            if (waitingForKey == 2) {
                ImGui::Text("[...]");
            } else {
                std::string espTxt = "[" + KeyToStr(espHotkey) + "]";
                ImGui::Text("%s", espTxt.c_str());
                if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::IsItemClicked()) waitingForKey = 2;
            }

            ImGui::SameLine(0.0f, 15.0f);
            ImGui::Text("Color:"); ImGui::SameLine();
            ImGui::ColorEdit3("##esp_def", espDefaultColor, clrFlags); ImGui::SameLine();
            ImGui::ColorEdit3("##esp_dan", espDangerColor, clrFlags);

            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 8.0f));

            CustomXCheckbox("Visibility Check", &visCheckEnabled);

            // Edge Scan
            CustomXCheckbox("Edge Scan (Multipoint)", &edgeScanEnabled);
            if (edgeScanEnabled) {
                ImGui::Text("Scan Radius:");
                ImGui::SetNextItemWidth(60.0f);
                ImGui::InputFloat("##esr_in", &edgeScanRadius, 0.0f, 0.0f, "%.1f");
                if (ImGui::IsItemDeactivatedAfterEdit()) { if (edgeScanRadius < 1.0f) edgeScanRadius=1.0f; if (edgeScanRadius > 28.0f) edgeScanRadius=28.0f; }
                ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
                CustomSliderFloat("##esr_sl", &edgeScanRadius, 1.0f, 28.0f);

                ImGui::Text("Scan Points:");
                ImGui::SetNextItemWidth(60.0f);
                ImGui::InputFloat("##esp_in", &edgeScanPoints, 0.0f, 0.0f, "%.0f");
                if (ImGui::IsItemDeactivatedAfterEdit()) { if (edgeScanPoints < 4.0f) edgeScanPoints=4.0f; if (edgeScanPoints > 32.0f) edgeScanPoints=32.0f; }
                ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
                CustomSliderFloat("##esp_sl", &edgeScanPoints, 4.0f, 32.0f);
            }

            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 8.0f));

            ImGui::Text("ESP Danger Distance:");
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputFloat("##dd_in", &espDangerDistance, 0.0f, 0.0f, "%.0f");
            if (ImGui::IsItemDeactivatedAfterEdit()) { if (espDangerDistance < 100) espDangerDistance=100; if (espDangerDistance > 3000) espDangerDistance=3000; }
            ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
            CustomSliderFloat("##dd_sl", &espDangerDistance, 100.0f, 3000.0f);

            ImGui::Text("AFK Timeout (ms):");
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputFloat("##afk_in", &afkTimeoutMs, 0.0f, 0.0f, "%.0f");
            if (ImGui::IsItemDeactivatedAfterEdit()) { if (afkTimeoutMs < 100) afkTimeoutMs=100; if (afkTimeoutMs > 10000) afkTimeoutMs=10000; }
            ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
            CustomSliderFloat("##afk_sl", &afkTimeoutMs, 100.0f, 10000.0f);

            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 8.0f));

            ImGui::Text("Prediction Factor:");
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputFloat("##pred_in", &predictionFactor, 0.0f, 0.0f, "%.2f");
            if (ImGui::IsItemDeactivatedAfterEdit()) { if (predictionFactor < 0.0f) predictionFactor = 0.0f; if (predictionFactor > 0.3f) predictionFactor = 0.3f; }
            ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
            CustomSliderFloat("##pred_sl", &predictionFactor, 0.0f, 0.3f);

            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 8.0f));

            ImGui::Text("Aimbot Mode:");
            ImGui::SetNextItemWidth(240.0f);
            const char* modeName = (aimbotMode == 0) ? "Classic" : "OnKey";
            if (ImGui::BeginCombo("##mode", modeName, ImGuiComboFlags_NoArrowButton)) {
                if (ImGui::Selectable("Classic", aimbotMode == 0)) aimbotMode = 0;
                if (ImGui::Selectable("OnKey", aimbotMode == 1)) aimbotMode = 1;
                ImGui::EndCombo();
            }

            ImGui::Spacing();

            if (aimbotMode == 0) {
                CustomXCheckbox("Show Classic FOV", &showClassicFOV);
                ImGui::SameLine(180); ImGui::ColorEdit3("##c_cfov", classicFovColor, clrFlags);

                ImGui::Text("Classic FOV Size:");
                ImGui::SetNextItemWidth(60.0f);
                ImGui::InputFloat("##cf_in", &classicFOV, 0.0f, 0.0f, "%.0f");
                if (ImGui::IsItemDeactivatedAfterEdit()) { if (classicFOV < 10) classicFOV=10; if (classicFOV > 1500) classicFOV=1500; }
                ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
                CustomSliderFloat("##cf_sl", &classicFOV, 10.0f, 1500.0f);
            } else {
                CustomXCheckbox("Show Hook FOV", &showHookFOV);
                ImGui::SameLine(160); ImGui::ColorEdit3("##c_hfov", hookFovColor, clrFlags);
                ImGui::SameLine(200);
                CustomXCheckbox("Show Laser FOV", &showLaserFOV);
                ImGui::SameLine(360); ImGui::ColorEdit3("##c_lfov", laserFovColor, clrFlags);

                CustomXCheckbox("FNG Mode", &fngMode);
                if (fngMode) {
                    ImGui::Indent(20.0f);
                    CustomXCheckbox("Auto Laser (Triggerbot)", &triggerBotEnabled);
                    ImGui::Unindent(20.0f);
                }

                ImGui::Text("Hook FOV (Max 380):");
                ImGui::SetNextItemWidth(60.0f);
                ImGui::InputFloat("##hf_in", &hookFOV, 0.0f, 0.0f, "%.0f");
                if (ImGui::IsItemDeactivatedAfterEdit()) { if (hookFOV < 10) hookFOV=10; if (hookFOV > 380) hookFOV=380; }
                ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
                CustomSliderFloat("##hf_sl", &hookFOV, 10.0f, 380.0f);

                ImGui::Text("Laser FOV (Max 800):");
                ImGui::SetNextItemWidth(60.0f);
                ImGui::InputFloat("##lf_in", &laserFOV, 0.0f, 0.0f, "%.0f");
                if (ImGui::IsItemDeactivatedAfterEdit()) { if (laserFOV < 10) laserFOV=10; if (laserFOV > 800) laserFOV=800; }
                ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
                CustomSliderFloat("##lf_sl", &laserFOV, 10.0f, 800.0f);
            }

            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 8.0f));

            CustomXCheckbox("Cone FOV (Angle limit)", &coneEnabled);
            if (coneEnabled) {
                CustomXCheckbox("Show Cone FOV", &showConeFOV);
                ImGui::SameLine(180); ImGui::ColorEdit3("##c_cone", coneColor, clrFlags);

                ImGui::Text("Cone Angle:");
                ImGui::SetNextItemWidth(60.0f);
                ImGui::InputFloat("##ca_in", &coneAngle, 0.0f, 0.0f, "%.0f");
                if (ImGui::IsItemDeactivatedAfterEdit()) { if (coneAngle < 1) coneAngle=1; if (coneAngle > 360) coneAngle=360; }
                ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
                CustomSliderFloat("##ca_sl", &coneAngle, 1.0f, 360.0f);

                ImGui::Text("Cone Show Length:");
                ImGui::SetNextItemWidth(60.0f);
                ImGui::InputFloat("##cl_in", &coneLength, 0.0f, 0.0f, "%.0f");
                if (ImGui::IsItemDeactivatedAfterEdit()) { if (coneLength < 10) coneLength=10; if (coneLength > 1000) coneLength=1000; }
                ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
                CustomSliderFloat("##cl_sl", &coneLength, 10.0f, 1000.0f);
            }

            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 8.0f));

            CustomXCheckbox("Spin Bot (For Fun!)", &spinBotEnabled);
            if (spinBotEnabled) {
                ImGui::Text("Spin Speed:");
                ImGui::SetNextItemWidth(60.0f);
                ImGui::InputFloat("##spin_in", &spinBotSpeed, 0.0f, 0.0f, "%.1f");
                if (ImGui::IsItemDeactivatedAfterEdit()) { if (spinBotSpeed < 1.0f) spinBotSpeed = 1.0f; if (spinBotSpeed > 50.0f) spinBotSpeed = 50.0f; }
                ImGui::SameLine(); ImGui::SetNextItemWidth(240.0f);
                CustomSliderFloat("##spin_sl", &spinBotSpeed, 1.0f, 50.0f);
            }

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    o_SDL_GL_SwapWindow(window);
}
