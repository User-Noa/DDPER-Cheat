#include "triggerbot.h"
#include "cheat.h"
#include "offsets.h"
#include "memory.h"

void HandleTriggerBot(uint32_t currentTime, bool hasTarget, int currentWeapon, uintptr_t aimBaseAddr) {
    if (!fngMode || !triggerBotEnabled) return;
    if (!changeGunEnabled) return;          
    
    if (!hasTarget) {
        if (needsWeaponSwitchBack) {
            if (autoFallbackEnabled) {
                WriteMem<int>(aimBaseAddr + OFF_AIM_WEAPON, prevWeaponID + 1);
                WriteMem<float>(aimBaseAddr + OFF_AIM_X, savedAimX);
                WriteMem<float>(aimBaseAddr + OFF_AIM_Y, savedAimY);
            }
            needsWeaponSwitchBack = false;   
        }
        return;
    }

    if (!needsWeaponSwitchBack) {
        prevWeaponID = currentWeapon;
        savedAimX = ReadMem<float>(aimBaseAddr + OFF_AIM_X);
        savedAimY = ReadMem<float>(aimBaseAddr + OFF_AIM_Y);
        needsWeaponSwitchBack = true;
    }

    if (currentWeapon != 4) {
        if (currentTime - lastSwitchTime > 150) {
            WriteMem<int>(aimBaseAddr + OFF_AIM_WEAPON, 5);
            lastSwitchTime = currentTime;
        }
        return;     
    }

    if (currentTime - lastShootTime > 800 && currentTime - lastSwitchTime > 50) {
        int currentShootVal = ReadMem<int>(aimBaseAddr + OFF_AIM_SHOOT);
        WriteMem<int>(aimBaseAddr + OFF_AIM_SHOOT, currentShootVal + 2);
        lastShootTime = currentTime;
    }
}