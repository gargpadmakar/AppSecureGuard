/**
 * ==================================================
 * guard.c
 * --------------------------------------------------
 * JNI entry + enforcement layer
 *
 * PURPOSE:
 *  - Acts as the ONLY bridge between Java ↔ Native
 *  - No detection logic here
 *  - Only decision making & enforcement
 *
 * SECURITY DESIGN:
 *  - Detection logic lives in core/ & security/
 *  - JNI layer stays thin (harder to reverse)
 * ==================================================
 */

#include <jni.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

/* --------------------------------------------------
 * Internal headers (MODULARIZED)
 * --------------------------------------------------
 * */
#include "../core/anti_debug.h"

#include "../security/root_check.h"
#include "../security/emulator_check.h"
#include "../security/magisk_check.h"
#include "../security/xposed_check.h"
#include "../security/frida_check.h"
#include "../security/mitm_check.h"


/* --------------------------------------------------
 * RISK MASK FLAGS (UI MAPPING)
 * -------------------------------------------------- */
#define RISK_DEV_OPTIONS   (1 << 0)
#define RISK_HOOK          (1 << 1)
#define RISK_NETWORK       (1 << 2)
#define RISK_INTEGRITY     (1 << 3)



/* --------------------------------------------------
 * GLOBAL STATE
 * --------------------------------------------------
 * Used to inform Java layer that networking
 * should be blocked (if queried)
 */
static int g_block_network = 0;

/* ==================================================
 * JNI: Check if network is blocked
 * ================================================== */
JNIEXPORT jboolean JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeIsNetworkBlocked(
        JNIEnv *env, jclass clazz) {

    return g_block_network ? JNI_TRUE : JNI_FALSE;
}

/* ==================================================
 * JNI: Hook / instrumentation detection
 * --------------------------------------------------
 * Lightweight check for runtime use
 * (no killing here)
 * ================================================== */
JNIEXPORT jboolean JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeIsHookDetected(
        JNIEnv *env, jclass clazz) {

    return
            is_debugger_present() ||
            detect_frida() ||
            detect_magisk() ||
            detect_xposed() ||
            detect_proxy_env() ||
            detect_mitm_ports();
}

/* ==================================================
 * JNI: Fake API response
 * --------------------------------------------------
 * Can be used to confuse attackers
 * when network is blocked
 * ================================================== */
JNIEXPORT jstring JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeFakeApiResponse(
        JNIEnv *env, jclass clazz) {

    return (*env)->NewStringUTF(
            env,
            "{\"status\":\"ok\",\"message\":\"No data\",\"data\":[]}"
    );
}

/* ==================================================
 * JNI: MAIN ENFORCEMENT FUNCTION
 * --------------------------------------------------
 * Called from Java (Application / Splash)
 *
 * BEHAVIOR:
 *  - Calculates native risk score
 *  - Combines Java + Native signals
 *  - On ANY risk → hard kill
 * ================================================== */
JNIEXPORT void JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeVerifyAndEnforce(
        JNIEnv *env,
        jclass clazz,
        jint javaRiskScore) {

    int risk = 0;

    /* -------- Root / system integrity -------- */
    if (detect_test_keys())        risk++;
    if (detect_su_binary())        risk++;
    if (detect_usb_debugging())    risk++;

    /* -------- Emulator -------- */
    if (detect_emulator())         risk += 2;

    /* -------- Runtime modification -------- */
    if (detect_magisk())           risk += 2;
    if (detect_xposed())           risk += 2;
    if (detect_frida())            risk += 3;

    /* -------- Network tampering -------- */
    if (detect_proxy_env())        risk += 2;
    if (detect_mitm_ports())       risk += 3;

    /* -------- Debugging -------- */
    if (is_debugger_present())     risk += 3;

    /* --------------------------------------------------
     * ENFORCEMENT
     * --------------------------------------------------
     * Any non-zero score = hostile environment
     */
    if (javaRiskScore > 0 || risk > 0) {

        g_block_network = 1;

        /* Hard process kill (no catch, no ANR) */
        kill(getpid(), SIGKILL);

        /* Fallback safety */
        _exit(0);
    }
}

/* ==================================================
 * JNI: Should show security alert
 * --------------------------------------------------
 * Used when you want popup + exit
 * instead of silent kill
 * ================================================== */
JNIEXPORT jboolean JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeShouldShowSecurityAlert(
        JNIEnv *env, jclass clazz) {

    return
            detect_test_keys() ||
            detect_su_binary() ||
            detect_usb_debugging() ||
            detect_emulator() ||
            detect_magisk() ||
            detect_xposed() ||
            detect_frida() ||
            detect_proxy_env() ||
            detect_mitm_ports() ||
            is_debugger_present();
}

/* ==================================================
 * JNI: Force exit (Java-triggered)
 * ================================================== */
JNIEXPORT void JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_forceExit(
        JNIEnv *env, jclass clazz) {

    kill(getpid(), SIGKILL);
    _exit(0);
}
/* ==================================================
 * JNI: Get risk mask (UI mapping only)
 * --------------------------------------------------
 * - NO enforcement
 * - NO killing
 * - Used only for UI messaging
 * ================================================== */
JNIEXPORT jint JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeGetRiskMask(
        JNIEnv *env, jclass clazz) {

    int mask = 0;

    /* -------- Low severity -------- */
    if (detect_usb_debugging() || detect_test_keys()) {
        mask |= RISK_DEV_OPTIONS;
    }

    /* -------- Hook / instrumentation -------- */
    if (detect_frida() || detect_xposed() || is_debugger_present()) {
        mask |= RISK_HOOK;
    }

    /* -------- Network tampering -------- */
    if (detect_proxy_env() || detect_mitm_ports()) {
        mask |= RISK_NETWORK;
    }

    /* -------- Device integrity -------- */
    if (detect_su_binary() || detect_emulator() || detect_magisk()) {
        mask |= RISK_INTEGRITY;
    }

    return mask;
}

/* ==================================================
 * JNI_OnLoad
 * --------------------------------------------------
 * Runs ONCE when library is loaded
 * Ideal place for:
 *  - ptrace()
 *  - self-protection init
 * ================================================== */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {

    /* Anti-debug init (ptrace) */
    init_ptrace();

    return JNI_VERSION_1_6;
}
/* ==================================================
 * JNI: Get risk messages (UI ONLY)
 * ================================================== */
JNIEXPORT jobjectArray JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeGetRiskMessages(
        JNIEnv *env, jclass clazz) {

    const char *messages[5];
    int count = 0;

    if (detect_frida() || detect_xposed() || is_debugger_present()) {
        messages[count++] = "• Debugging or hooking tools detected";
    }

    if (detect_proxy_env() || detect_mitm_ports()) {
        messages[count++] = "• Proxy, VPN, or network interception detected";
    }

    if (detect_usb_debugging() || detect_test_keys()) {
        messages[count++] = "• Developer options or USB debugging is enabled";
    }

    if (detect_su_binary() || detect_emulator() || detect_magisk()) {
        messages[count++] = "• Device integrity issue detected";
    }

    if (count == 0) {
        messages[count++] = "• Suspicious environment detected";
    }

    jclass stringClass = (*env)->FindClass(env, "java/lang/String");
    jobjectArray array = (*env)->NewObjectArray(env, count, stringClass, NULL);

    for (int i = 0; i < count; i++) {
        (*env)->SetObjectArrayElement(
                env,
                array,
                i,
                (*env)->NewStringUTF(env, messages[i])
        );
    }

    return array;
}
JNIEXPORT jstring JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeGetSecurityHeader(
        JNIEnv *env, jclass clazz) {

    return (*env)->NewStringUTF(
            env,
            "Security notice:\n\n"
    );
}
JNIEXPORT jstring JNICALL
Java_com_padmakar_secureguard_AppProtectGuard_nativeGetSecurityFooter(
        JNIEnv *env, jclass clazz) {

    return (*env)->NewStringUTF(
            env,
            "\nThe application will close to protect your data."
    );
}
