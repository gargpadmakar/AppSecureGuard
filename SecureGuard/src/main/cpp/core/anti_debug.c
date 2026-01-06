//
// Created by Dell on 1/6/2026.
//
#include <sys/ptrace.h>
#include <errno.h>

int detect_debugger() {
    errno = 0;
    return ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1;
}
