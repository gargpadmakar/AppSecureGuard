//
// Created by Dell on 1/6/2026.
//
#include <unistd.h>

int detect_root() {
    return access("/system/bin/su", F_OK) == 0 ||
           access("/system/xbin/su", F_OK) == 0 ||
           access("/sbin/su", F_OK) == 0;
}
