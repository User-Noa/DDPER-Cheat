#include "triggerbot.h"
#include "cheat.h"
#include "input.h"

void HandleTriggerBot(uint32_t currentTime, bool hasTarget, int currentWeapon) {
    if (!fngMode || !triggerBotEnabled) return;
    if (!hasTarget) return;

    if (currentWeapon != 4) { // اگر لیزر نیست
        if (currentTime - lastSwitchTime > 150 && currentTime - lastShootTime > 800) {
            prevWeaponID = currentWeapon;
           
            SimulateKeyClick(SDLK_5); // دکمه 5 برای لیزر
            lastSwitchTime = currentTime;
            needsWeaponSwitchBack = true;
        }
    } else { // لیزر دستش است
        if (currentTime - lastSwitchTime > 50 && currentTime - lastShootTime > 800) {
            SimulateMouseLeftClick();
            lastShootTime = currentTime;
        }
    }

    // برگشت به سلاح قبلی
    if (needsWeaponSwitchBack) {
        if (currentTime - lastShootTime > 100 && currentTime - lastShootTime < 400) {
            if (prevWeaponID >= 0 && prevWeaponID <= 5) {
                SimulateKeyClick(SDLK_1 + prevWeaponID);
            }
            needsWeaponSwitchBack = false;
        }
    }
}


