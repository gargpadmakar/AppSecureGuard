//
// Created by Dell on 1/7/2026.
//
#include <stdio.h>
#include <string.h>
#include "magisk_check.h"
#include "../common/utils.h"

int detect_magisk() {

    // Classic / common paths
    if (file_exists("/sbin/magisk")) return 1;
    if (file_exists("/sbin/.magisk")) return 1;
    if (file_exists("/cache/.magisk")) return 1;
    if (file_exists("/data/adb/magisk")) return 1;

    // Modules / Zygisk
    if (file_exists("/data/adb/modules")) return 1;
    if (file_exists("/data/adb/modules_update")) return 1;
    if (file_exists("/data/adb/zygisk")) return 1;

    // Riru helpers
    if (file_exists("/data/adb/riru")) return 1;
    if (file_exists("/data/adb/modules/riru-core")) return 1;

    // Hidden binaries
    if (file_exists("/system/bin/magisk")) return 1;
    if (file_exists("/system/xbin/magisk")) return 1;
    if (file_exists("/system/bin/.magisk")) return 1;

    // OverlayFS / mounts
    FILE *mounts = fopen("/proc/self/mounts", "r");
    if (mounts) {
        char line[512];
        while (fgets(line, sizeof(line), mounts)) {
            if ((strstr(line, "magisk") || strstr(line, "overlay")) &&
                (strstr(line, "/system") || strstr(line, "/vendor"))) {
                fclose(mounts);
                return 1;
            }
        }
        fclose(mounts);
    }

    // Runtime injection
    FILE *maps = fopen("/proc/self/maps", "r");
    if (maps) {
        char line[512];
        while (fgets(line, sizeof(line), maps)) {
            if (strstr(line, "magisk") ||
                strstr(line, "zygisk")) {
                fclose(maps);
                return 1;
            }
        }
        fclose(maps);
    }

    return 0;
}
