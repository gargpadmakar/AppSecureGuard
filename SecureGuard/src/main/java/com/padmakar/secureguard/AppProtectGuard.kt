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
    // 🔥 HARD MODE (Enterprise / Banking)
    // =================================================
    // MUST be called early (Application / Splash)
    @JvmStatic
    fun enforce(context: Context) {
        var javaRiskScore = 0

        // 🔐 Native hook / instrumentation check
        if (nativeIsHookDetected()) {
            javaRiskScore += 3
        }

        // 🔥 Native decides final enforcement
        nativeVerifyAndEnforce(javaRiskScore)
    }

    // =================================================
    // 🔔 SOFT MODE (Play Store safe)
    // =================================================
    // BottomSheet based alert – NO finish(), NO kill
    @JvmStatic
    fun showSecurityAlertIfNeeded(activity: FragmentActivity) {
        if (shouldShowSecurityAlert()) {
            SecurityAlertBottomSheet()
                .show(activity.supportFragmentManager, "SecurityAlert")
        }
    }

    // =================================================
    // 🔔 POPUP SIGNAL (PURE NATIVE)
    // =================================================
    private external fun nativeShouldShowSecurityAlert(): Boolean

    @JvmStatic
    fun shouldShowSecurityAlert(): Boolean {
        return nativeShouldShowSecurityAlert()
    }

    // =================================================
    // 🌐 NETWORK / FAKE RESPONSE SUPPORT
    // =================================================
    @JvmStatic
    fun isNetworkBlocked(): Boolean {
        return nativeIsNetworkBlocked()
    }

    @JvmStatic
    fun shouldUseFakeResponse(): Boolean {
        return isNetworkBlocked()
    }

    @JvmStatic
    fun getFakeApiResponse(): String {
        return nativeFakeApiResponse()
    }

    // =================================================
    // 🔥 JNI BRIDGE (PRIVATE)
    // =================================================
    private external fun nativeVerifyAndEnforce(javaRiskScore: Int)
    private external fun nativeIsHookDetected(): Boolean
    private external fun nativeIsNetworkBlocked(): Boolean
    private external fun nativeFakeApiResponse(): String

    // =================================================
    // ❌ FORCE EXIT (Optional – Java triggered)
    // =================================================
    @JvmStatic
    external fun forceExit()

    // INTERNAL use for UI only
    internal fun isHookDetectedInternal(): Boolean {
        return try {
            val m = AppProtectGuard::class.java
                .getDeclaredMethod("nativeIsHookDetected")
            m.isAccessible = true
            m.invoke(null) as Boolean
        } catch (e: Throwable) {
            false
        }
    }

}
