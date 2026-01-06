# Keep JNI bindings
-keepclasseswithmembernames class * {
    native <methods>;
}

-keep class com.padmakar.secureguard.** { *; }
