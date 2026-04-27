#include "visibility.h"
#include "memory.h"
#include <cmath>
#include <set>

static const std::set<uint8_t> PASSABLE_TILES = {
    0x00,   // Air
    0x09,   // Void
    0xC0,
    0x15,
    0x02,
    0x0B,
    0xBF,
    0xBE,
    0x21,
    0x0D,
    0x16,
    0x5A,
    0x6A,
    0x13,
    0x14
};

extern bool visCheckEnabled;

bool IsVisible(float x1, float y1, float x2, float y2, int mapW, int mapH, uintptr_t tilesBase) {
    if (!visCheckEnabled) return true;                     
    if (!tilesBase || mapW <= 0 || mapH <= 0) return true;

    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = std::sqrt(dx*dx + dy*dy);
    if (distance == 0.0f) return true;

    float stepX = dx / distance;
    float stepY = dy / distance;

    int lastTx = -1, lastTy = -1;

    for (float i = 0; i <= distance; i += 4.0f) {
        float cx = x1 + stepX * i;
        float cy = y1 + stepY * i;

        int tx = static_cast<int>(cx / 32.0f);
        int ty = static_cast<int>(cy / 32.0f);

        if (tx == lastTx && ty == lastTy) continue;
        lastTx = tx; lastTy = ty;

        if (tx >= 0 && tx < mapW && ty >= 0 && ty < mapH) {
            uint8_t tileIndex = ReadMem<uint8_t>(tilesBase + (ty * mapW + tx) * 4);
            if (PASSABLE_TILES.find(tileIndex) == PASSABLE_TILES.end())
                return false;
        }
    }
    return true;
}