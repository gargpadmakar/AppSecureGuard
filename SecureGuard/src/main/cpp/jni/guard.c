//
// Created by Dell on 1/6/2026.
//
#include <jni.h>
#include <unistd.h>
#include <signal.h>

int detect_root();
int detect_debugger();
int detect_frida();
int detect_proxy_env();

static int g_block_network = 0;

JNIEXPORT void JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeVerifyAndEnforce(
        JNIEnv *env, jclass clazz, jint score) {

int risk = 0;
if (detect_root()) risk++;
if (detect_debugger()) risk++;
if (detect_frida()) risk += 2;
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
    return detect_root() || detect_frida();
}

JNIEXPORT jboolean JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeIsHookDetected(
        JNIEnv *env, jclass clazz) {
    return detect_debugger() || detect_frida();
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
