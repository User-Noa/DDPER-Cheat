#include "spinbot.h"
#include "cheat.h"
#include "memory.h"
#include <cmath>

void HandleSpinBot(uintptr_t aimBaseAddr, bool shouldSpin) {
    if (!shouldSpin || !spinBotEnabled) return;

    currentSpinAngle += spinBotSpeed * 0.05f;
    if (currentSpinAngle > 2.0f * PI) currentSpinAngle -= 2.0f * PI;

    float sdx = 100.0f * cosf(currentSpinAngle);
    float sdy = 100.0f * sinf(currentSpinAngle);

    WriteMem<float>(aimBaseAddr + 0x10, sdx);
    WriteMem<float>(aimBaseAddr + 0x14, sdy);
}
