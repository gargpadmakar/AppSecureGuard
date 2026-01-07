package com.padmakar.secureguard

import android.content.Context
import androidx.fragment.app.FragmentActivity

/**
 * ==================================================
 * AppProtectGuard
 * --------------------------------------------------
 * Public security entry point for app module
 *
 * DESIGN:
 *  - Native (JNI) is the source of truth
 *  - App module never talks to JNI directly
 *  - Soft mode  -> BottomSheet based alert
 *  - Hard mode  -> Native enforce (SIGKILL)
 * ==================================================
 */
object AppProtectGuard {

    init {
        System.loadLibrary("secureguard")
    }

    // =================================================
    // 🧩 RISK MASK FLAGS (MUST MATCH guard.c)
    // =================================================
    const val RISK_DEV_OPTIONS = 1 shl 0
    const val RISK_HOOK        = 1 shl 1
    const val RISK_NETWORK     = 1 shl 2
    const val RISK_INTEGRITY   = 1 shl 3

    // =================================================
    // 🔥 HARD MODE (Enterprise / Banking)
    // =================================================
    @JvmStatic
    fun enforce(context: Context) {
        var javaRiskScore = 0

        // High-severity native signal
        if (nativeIsHookDetected()) {
            javaRiskScore += 3
        }

        // Native decides final enforcement
        nativeVerifyAndEnforce(javaRiskScore)
    }

    // =================================================
    // 🔔 SOFT MODE (Play Store safe)
    // =================================================
    @JvmStatic
    fun showSecurityAlertIfNeeded(activity: FragmentActivity) {
        if (shouldShowSecurityAlert()) {
            SecurityAlertBottomSheet()
                .show(activity.supportFragmentManager, "SecurityAlert")
        }
    }

    // =================================================
    // 🔔 GENERIC NATIVE SIGNAL
    // =================================================
    private external fun nativeShouldShowSecurityAlert(): Boolean

    @JvmStatic
    fun shouldShowSecurityAlert(): Boolean {
        return nativeShouldShowSecurityAlert()
    }

    // =================================================
    // 🌐 NETWORK / MITM
    // =================================================
    private external fun nativeIsNetworkBlocked(): Boolean

    @JvmStatic
    fun isNetworkBlocked(): Boolean {
        return nativeIsNetworkBlocked()
    }

    @JvmStatic
    fun shouldUseFakeResponse(): Boolean = isNetworkBlocked()

    @JvmStatic
    fun getFakeApiResponse(): String = nativeFakeApiResponse()

    // =================================================
    // 🔴 HIGH-SEVERITY: HOOK / DEBUGGER
    // =================================================
    private external fun nativeIsHookDetected(): Boolean

    /**
     * INTERNAL (UI only)
     * Used by BottomSheet if needed
     */
    internal fun isHookDetectedInternal(): Boolean {
        return nativeIsHookDetected()
    }

    // =================================================
    // 🧠 NATIVE RISK MASK (UI MAPPING)
    // =================================================
    private external fun nativeGetRiskMask(): Int

    /**
     * Returns native-calculated risk bitmask
     * Used ONLY for UI messaging
     */
    @JvmStatic
    fun getRiskMask(): Int {
        return nativeGetRiskMask()
    }
    private external fun nativeGetRiskMessages(): Array<String>

    @JvmStatic
    fun getRiskMessages(): List<String> {
        return nativeGetRiskMessages().toList()
    }

    // Header & footer from native
    private external fun nativeGetSecurityHeader(): String
    private external fun nativeGetSecurityFooter(): String

    @JvmStatic
    fun getSecurityHeader(): String = nativeGetSecurityHeader()

    @JvmStatic
    fun getSecurityFooter(): String = nativeGetSecurityFooter()

    // =================================================
    // 🔥 JNI CORE
    // =================================================
    private external fun nativeVerifyAndEnforce(javaRiskScore: Int)
    private external fun nativeFakeApiResponse(): String

    // =================================================
    // ❌ FORCE EXIT (SIGKILL)
    // =================================================
    @JvmStatic
    external fun forceExit()
}
