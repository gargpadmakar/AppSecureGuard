//
// Created by Dell on 1/7/2026.
//
#include <stdio.h>
#include <string.h>
#include "xposed_check.h"
#include "../common/utils.h"

int detect_xposed() {

    // Legacy Xposed
    if (file_exists("/system/framework/XposedBridge.jar")) return 1;
    if (file_exists("/system/lib/libxposed_art.so")) return 1;
    if (file_exists("/system/lib64/libxposed_art.so")) return 1;
    if (file_exists("/system/bin/app_process_xposed")) return 1;
    if (file_exists("/system/bin/app_process64_xposed")) return 1;

    // EdXposed
    if (file_exists("/data/adb/modules/edxposed")) return 1;
    if (file_exists("/data/adb/modules/riru_edxposed")) return 1;
    if (file_exists("/data/adb/edxp")) return 1;

    // LSPosed / Zygisk
    if (file_exists("/data/adb/modules/lsposed")) return 1;
    if (file_exists("/data/adb/lspd")) return 1;
    if (file_exists("/data/adb/modules/zygisk_lsposed")) return 1;
    if (file_exists("/data/adb/zygisk")) return 1;

    // Riru
    if (file_exists("/data/adb/riru")) return 1;
    if (file_exists("/data/adb/modules/riru-core")) return 1;

    // Runtime injected libs
    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp) {
        char line[512];
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line,"xposed") ||
                strstr(line,"lsposed") ||
                strstr(line,"edxp") ||
                strstr(line,"lspd") ||
                strstr(line,"riru")) {
                fclose(fp);
                return 1;
            }
        }
        fclose(fp);
    }
    return 0;
}
