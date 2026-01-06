#include <jni.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ==================================================
 * GLOBAL STATE
 * ================================================== */
static int g_block_network = 0;

/* ==================================================
 * BASIC DETECTIONS (SAFE STUBS)
 * 👉 You can harden later
 * ================================================== */

static int detect_root() {
    return access("/system/bin/su", F_OK) == 0 ||
           access("/system/xbin/su", F_OK) == 0 ||
           access("/sbin/su", F_OK) == 0;
}

static int detect_debugger() {
    FILE *fp = fopen("/proc/self/status", "r");
    if (!fp) return 0;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "TracerPid:", 10) == 0) {
            int pid = atoi(line + 10);
            fclose(fp);
            return pid > 0;
        }
    }
    fclose(fp);
    return 0;
}

static int detect_frida() {
    FILE *fp = fopen("/proc/self/maps", "r");
    if (!fp) return 0;

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "frida") || strstr(line, "gum-js-loop")) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

static int detect_proxy_env() {
    const char *vars[] = {
            "http_proxy", "https_proxy",
            "HTTP_PROXY", "HTTPS_PROXY",
            "ALL_PROXY"
    };

    for (int i = 0; i < 5; i++) {
        char *v = getenv(vars[i]);
        if (v && strlen(v) > 0) return 1;
    }
    return 0;
}

/* ==================================================
 * JNI METHODS (MATCH Kotlin EXACTLY)
 * ================================================== */

JNIEXPORT void JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeVerifyAndEnforce(
        JNIEnv *env, jclass clazz, jint score) {

int risk = 0;
if (detect_root())      risk++;
if (detect_debugger())  risk++;
if (detect_frida())     risk += 2;
if (detect_proxy_env()) risk++;

if (score > 0 || risk > 0) {
g_block_network = 1;
kill(getpid(), SIGKILL);
_exit(0);
}
}

JNIEXPORT jboolean JNICALL
        Java_com_padmakar_secureguard_AppProtectGuard_nativeShouldShowSecurityAlert(
        JNIEnv *env, jclass clazz) {
return (detect_root() || detect_frida()) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
        Java_com_padmakar_secureguard_AppProtectGuard_nativeIsHookDetected(
        JNIEnv *env, jclass clazz) {
return (detect_debugger() || detect_frida()) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
        Java_com_padmakar_secureguard_AppProtectGuard_nativeIsNetworkBlocked(
        JNIEnv *env, jclass clazz) {
return g_block_network ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
        Java_com_padmakar_secureguard_AppProtectGuard_nativeFakeApiResponse(
        JNIEnv *env, jclass clazz) {
return (*env)->NewStringUTF(env,
"{\"status\":\"ok\",\"data\":[]}");
}

JNIEXPORT void JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_forceExit(
        JNIEnv *env, jclass clazz) {
kill(getpid(), SIGKILL);
_exit(0);
}

/* ==================================================
 * JNI LOAD
 * ================================================== */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    return JNI_VERSION_1_6;
}

#ifdef __cplusplus
}
#endif
