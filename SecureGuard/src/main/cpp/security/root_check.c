//
// Created by Dell on 1/7/2026.
//
#include <stdio.h>
#include <string.h>
#include "root_check.h"
#include "../common/utils.h"

int detect_su_binary() {
    const char *paths[] = {
            "/sbin/su","/system/bin/su","/system/xbin/su",
            "/data/local/xbin/su","/data/local/bin/su",
            "/system/bin/failsafe/su","/su/bin/su",
            "/magisk/.core/bin/su"
    };
    for (int i = 0; i < (int)(sizeof(paths)/sizeof(paths[0])); i++)
        if (file_exists(paths[i])) return 1;
    return 0;
}

int detect_test_keys() {
    FILE *fp = popen("getprop ro.build.tags", "r");
    if (!fp) return 0;
    char buf[128];
    fgets(buf, sizeof(buf), fp);
    pclose(fp);
    return strstr(buf, "test-keys") != NULL;
}

int detect_usb_debugging() {
    FILE *fp = popen("getprop persist.sys.usb.config", "r");
    if (!fp) return 0;
    char buf[128];
    fgets(buf, sizeof(buf), fp);
    pclose(fp);
    return strstr(buf, "adb") != NULL;
}
