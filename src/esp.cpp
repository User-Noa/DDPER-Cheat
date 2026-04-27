#include "esp.h"
#include "cheat.h"
#include "offsets.h"
#include "memory.h"
#include "render.h"
#include <cmath>

void DrawESP(float myX, float myY, int myID, uintptr_t entityList, int screenW, int screenH, uint32_t currentTime) {
    float cx = screenW / 2.0f;
    float cy = screenH / 2.0f;

    for (int i = 0; i < 128; i++) {
        if (i == myID) continue;
        uintptr_t playerBlock = entityList + (uintptr_t)i * ENTITY_SIZE;
        float enX = ReadMem<float>(playerBlock + OFF_POS_X);
        float enY = ReadMem<float>(playerBlock + OFF_POS_Y);
        if (enX == 0 && enY == 0) continue;

        float enVelX = ReadMem<float>(playerBlock + OFF_VEL_X);
        float enVelY = ReadMem<float>(playerBlock + OFF_VEL_Y);
        float predictedX = enX + enVelX * predictionFactor;
        float predictedY = enY + enVelY * predictionFactor;

        float dx = predictedX - myX;
        float dy = predictedY - myY;
        float dist = std::sqrt(dx*dx + dy*dy);

        if (players[i].lastMoveTime == 0)
            players[i].lastMoveTime = currentTime;
        if (std::abs(enX - players[i].lastX) > 0.1f || std::abs(enY - players[i].lastY) > 0.1f) {
            players[i].lastX = enX;
            players[i].lastY = enY;
            players[i].lastMoveTime = currentTime;
        }
        bool isAlive = (currentTime - players[i].lastMoveTime) < (uint32_t)afkTimeoutMs;
        int isDead = ReadMem<int>(playerBlock + OFF_IS_DEAD);

        if (espEnabled && dist > 1.0f) {
            if (!isDead) {
                if (!isAlive) {
                    DrawLineModern(cx, cy, cx + dx, cy + dy, 0.3f, 0.3f, 0.3f, screenW, screenH);
                } else if (dist < espDangerDistance) {
                    DrawLineModern(cx, cy, cx + dx, cy + dy, espDangerColor[0], espDangerColor[1], espDangerColor[2], screenW, screenH);
                } else {
                    DrawLineModern(cx, cy, cx + dx, cy + dy, espDefaultColor[0], espDefaultColor[1], espDefaultColor[2], screenW, screenH);
                }
            }
        }
    }
}
