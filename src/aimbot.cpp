#include "aimbot.h"
#include "cheat.h"
#include "offsets.h"
#include "memory.h"
#include "visibility.h"
#include <cmath>

void FindAimbotTarget(float myX, float myY, int myID, uintptr_t entityList,
                      int mapW, int mapH, uintptr_t tilesBase,
                      float currentActiveFOV, bool shouldAimNow, float realAimAngle,
                      int& bestTargetID, float& bestX, float& bestY) {
    bestTargetID = -1;
    bestX = bestY = 0.0f;
    float bestDist = 999999.0f;

    uint32_t currentTime = SDL_GetTicks();

    for (int i = 0; i < 128; i++) {
        if (i == myID) continue;

        uintptr_t playerBlock = entityList + (uintptr_t)i * ENTITY_SIZE;
        float enX = ReadMem<float>(playerBlock + OFF_POS_X);
        float enY = ReadMem<float>(playerBlock + OFF_POS_Y);
        if (enX == 0 && enY == 0) continue;

        int isSolo = ReadMem<int>(playerBlock + OFF_IS_SOLO);
        if (aimbotMode == 0 && isSolo == 275) continue;

        int isFreeze = ReadMem<int>(playerBlock + OFF_FREEZE_END);
        int isDead   = ReadMem<int>(playerBlock + OFF_IS_DEAD);

        float enVelX = ReadMem<float>(playerBlock + OFF_VEL_X);
        float enVelY = ReadMem<float>(playerBlock + OFF_VEL_Y);

        float predictedX = enX + enVelX * predictionFactor;
        float predictedY = enY + enVelY * predictionFactor;

        if (players[i].lastMoveTime == 0)
            players[i].lastMoveTime = currentTime;
        if (std::abs(enX - players[i].lastX) > 0.1f || std::abs(enY - players[i].lastY) > 0.1f) {
            players[i].lastX = enX;
            players[i].lastY = enY;
            players[i].lastMoveTime = currentTime;
        }
        bool isAlive = (currentTime - players[i].lastMoveTime) < (uint32_t)afkTimeoutMs;

        float dx = predictedX - myX;
        float dy = predictedY - myY;
        float dist = std::sqrt(dx*dx + dy*dy);

        bool triggerBotShouldShoot = (fngMode && triggerBotEnabled && isFreeze == 0 && isDead == 0 && isAlive);

        if ((isAlive && dist > 1.0f && dist < currentActiveFOV && shouldAimNow) ||
            (triggerBotShouldShoot && dist < laserFOV)) {
            if (fngMode && leftMouseDown && (isFreeze != 0 || isDead != 0) && !triggerBotEnabled) {
                continue;
            }

            bool insideCone = true;
            if (coneEnabled) {
                float targetAngle = atan2(dy, dx);
                float diff = std::abs(realAimAngle - targetAngle);
                if (diff > PI) diff = 2.0f * PI - diff;
                if (diff > (coneAngle * PI / 180.0f) / 2.0f)
                    insideCone = false;
            }

            bool isVis = false;
            float finalAimX = predictedX;
            float finalAimY = predictedY;

            if (aimbotMode == 0 && isFreeze != 0 && !triggerBotShouldShoot) {
                isVis = true;
            } else if (IsVisible(myX, myY, predictedX, predictedY, mapW, mapH, tilesBase)) {
                isVis = true;
            } else if (edgeScanEnabled) {
                int points = static_cast<int>(edgeScanPoints);
                for (int a = 0; a < points; a++) {
                    float angle = (2.0f * PI / points) * a;
                    float sx = predictedX + cosf(angle) * edgeScanRadius;
                    float sy = predictedY + sinf(angle) * edgeScanRadius;
                    if (IsVisible(myX, myY, sx, sy, mapW, mapH, tilesBase)) {
                        isVis = true;
                        finalAimX = sx;
                        finalAimY = sy;
                        break;
                    }
                }
            }

            if (insideCone && isVis && dist < bestDist) {
                bestDist = dist;
                bestTargetID = i;
                bestX = finalAimX;
                bestY = finalAimY;
            }
        }
    }
}
