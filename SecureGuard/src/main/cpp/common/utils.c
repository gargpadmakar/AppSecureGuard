//
// Created by Dell on 1/6/2026.
//
#include <stdlib.h>
#include <string.h>

int detect_proxy_env() {
    const char *vars[] = {
            "http_proxy","https_proxy",
            "HTTP_PROXY","HTTPS_PROXY"
    };

    for (int i = 0; i < 4; i++) {
        char *v = getenv(vars[i]);
        if (v && strlen(v) > 0) return 1;
    }
    return 0;
}
