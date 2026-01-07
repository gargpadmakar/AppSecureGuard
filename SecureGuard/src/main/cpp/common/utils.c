//
// Created by Dell on 1/6/2026.
//
#include <sys/stat.h>
#include "utils.h"

int file_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0);
}

