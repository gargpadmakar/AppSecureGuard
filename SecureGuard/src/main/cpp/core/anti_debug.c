//
// Created by Dell on 1/6/2026.
//
#include <sys/ptrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "anti_debug.h"

static int g_ptrace_ok = -1;

void init_ptrace() {
    if (g_ptrace_ok != -1) return;
    errno = 0;
    g_ptrace_ok = (ptrace(PTRACE_TRACEME, 0, NULL, NULL) != -1);
}

int is_debugger_present() {
    if (!g_ptrace_ok) return 1;

    FILE *fp = fopen("/proc/self/status", "r");
    if (!fp) return 0;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (!strncmp(line, "TracerPid:", 10)) {
            int pid = atoi(line + 10);
            fclose(fp);
            return pid > 0;
        }
    }
    fclose(fp);
    return 0;
}


