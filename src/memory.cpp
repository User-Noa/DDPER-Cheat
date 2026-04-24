#include "memory.h"
#include <cstdio>
#include <cstring>

uintptr_t GetModuleBase(const char* module_name, int target_index) {
    FILE* f = fopen("/proc/self/maps", "r");
    if (!f) return 0;
    char line[512]; int index = 0; uintptr_t start_addr = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, module_name)) {
            if (index == target_index) { sscanf(line, "%lx", &start_addr); break; }
            index++;
        }
    }
    fclose(f); return start_addr;
}
