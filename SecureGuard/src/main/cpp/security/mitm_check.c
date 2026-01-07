//
// Created by Dell on 1/7/2026.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mitm_check.h"

int detect_proxy_env() {
    const char *vars[] = {
            "http_proxy","https_proxy",
            "HTTP_PROXY","HTTPS_PROXY",
            "ALL_PROXY","all_proxy"
    };
    for (int i = 0; i < (int)(sizeof(vars)/sizeof(vars[0])); i++) {
        char *v = getenv(vars[i]);
        if (v && strlen(v) > 0) return 1;
    }
    return 0;
}

int detect_mitm_ports() {
    FILE *fp = fopen("/proc/net/tcp", "r");
    if (!fp) return 0;
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, ":1F90") ||   // 8080
            strstr(line, ":22B8") ||   // 8888
            strstr(line, ":2382")) {   // 9090
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}
