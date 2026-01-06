object AppProtectGuard {

    init {
        System.loadLibrary("secureguard")
    }

    @JvmStatic external fun nativeVerifyAndEnforce(javaRiskScore: Int)
    @JvmStatic external fun nativeShouldShowSecurityAlert(): Boolean
    @JvmStatic external fun nativeIsHookDetected(): Boolean
    @JvmStatic external fun nativeIsNetworkBlocked(): Boolean
    @JvmStatic external fun nativeFakeApiResponse(): String
    @JvmStatic external fun forceExit()
}
