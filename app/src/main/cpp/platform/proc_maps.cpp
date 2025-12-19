#include "proc_maps.h"
#include <cstdio>
#include <string>

std::string read_proc_self_maps() {
    FILE* f = fopen("/proc/self/maps", "rb");
    if (!f) return {};

    std::string out;
    char buf[4096];

    while (true) {
        const size_t n = fread(buf, 1, sizeof(buf), f);
        if (n > 0) out.append(buf, n);
        if (n < sizeof(buf)) break;
    }

    fclose(f);
    return out;
}
