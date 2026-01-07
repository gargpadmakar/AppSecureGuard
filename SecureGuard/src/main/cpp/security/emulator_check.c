//
// Created by Dell on 1/7/2026.
//
#include <stdio.h>
#include <string.h>
#include "emulator_check.h"
#include "../common/utils.h"

int detect_emulator() {

    // QEMU / AVD
    if (file_exists("/dev/qemu_pipe")) return 1;
    if (file_exists("/dev/socket/qemud")) return 1;

    // VirtualBox / Genymotion
    if (file_exists("/dev/vboxguest")) return 1;
    if (file_exists("/dev/vboxuser")) return 1;

    // Emulator binaries
    const char *emu_files[] = {
            "/system/bin/qemu-props",
            "/system/bin/androVM-prop",
            "/system/bin/microvirt-prop",
            "/system/bin/nox-prop",
            "/system/lib/libhoudini.so",
            "/system/lib64/libhoudini.so"
    };
    for (int i = 0; i < (int)(sizeof(emu_files)/sizeof(emu_files[0])); i++)
        if (file_exists(emu_files[i])) return 1;

    // System properties
    const char *props[] = {
            "ro.kernel.qemu","ro.product.model",
            "ro.product.manufacturer","ro.hardware",
            "ro.build.fingerprint"
    };

    for (int i = 0; i < 5; i++) {
        char cmd[128];
        snprintf(cmd, sizeof(cmd), "getprop %s", props[i]);
        FILE *fp = popen(cmd, "r");
        if (!fp) continue;

        char buf[256] = {0};
        fgets(buf, sizeof(buf), fp);
        pclose(fp);

        if (strstr(buf,"generic") || strstr(buf,"sdk") ||
            strstr(buf,"emulator") || strstr(buf,"goldfish") ||
            strstr(buf,"ranchu") || strstr(buf,"Genymotion") ||
            strstr(buf,"vbox") || strstr(buf,"VirtualBox") ||
            strstr(buf,"nox") || strstr(buf,"bluestacks") ||
            strstr(buf,"memu") || strstr(buf,"LDPlayer") ||
            strstr(buf,"MuMu")) return 1;
    }

    // CPU hypervisor
    FILE *cpu = fopen("/proc/cpuinfo", "r");
    if (cpu) {
        char line[512];
        while (fgets(line, sizeof(line), cpu)) {
            if (strstr(line, "hypervisor") ||
                strstr(line, "qemu") ||
                strstr(line, "vbox")) {
                fclose(cpu);
                return 1;
            }
        }
        fclose(cpu);
    }
    return 0;
}
