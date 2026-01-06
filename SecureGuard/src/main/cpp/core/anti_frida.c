//
// Created by Dell on 1/6/2026.
//
#include <stdio.h>
#include <string.h>

int detect_frida() {
    FILE *fp = fopen("/proc/self/maps", "r");
    if (!fp) return 0;

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "frida") || strstr(line, "gum-js-loop")) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}
