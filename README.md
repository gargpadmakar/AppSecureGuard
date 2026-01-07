# 🛡️ AppProtectGuard

[![](https://jitpack.io/v/padmakar1811/AppSecureGuard.svg)](https://jitpack.io/#padmakar1811/AppSecureGuard)
![Platform](https://img.shields.io/badge/platform-Android-green)
![Language](https://img.shields.io/badge/language-Java%20%7C%20Kotlin%20%7C%20JNI-blue)
![Security](https://img.shields.io/badge/focus-Mobile%20Security-red)

**AppProtectGuard** is an Android security library that detects **compromised, tampered, or hostile runtime environments**.  
It uses a **native (JNI) security core** to significantly increase resistance against reverse engineering, hooking, and runtime bypass techniques.

> Designed for **banking, fintech, enterprise, DRM, and high-security Android applications**.

---

## ✨ Features

### 🔐 Device & System Integrity
- Root detection (`su`, Magisk, Zygisk)
- Test-keys (non-production Android builds)
- USB debugging detection

### 📱 Emulator Detection
- Android Emulator (AVD / QEMU)
- Genymotion, BlueStacks, Nox, LDPlayer, MEmu
- Hypervisor / VirtualBox detection

### 🧪 Runtime Instrumentation Detection
- Debugger detection (ptrace + `/proc`)
- Frida detection (memory maps, gum artifacts)
- Xposed / LSPosed / EdXposed / Riru detection

### 🌐 Network & MITM Protection
- HTTP / HTTPS proxy detection
- MITM interception ports (8080, 8888, 9090)
- Environment proxy variables detection

### 🧬 Native (JNI) Security Core
- Core checks implemented in C (JNI)
- Reduced Java-level attack surface
- Hard-kill enforcement for high-risk environments

---

## 📦 Installation

### 1️⃣ Add JitPack Repository

**`settings.gradle` or root `build.gradle`**
```gradle
repositories {
    maven { url "https://jitpack.io" }
}

```

### Step 2: Add Dependency

In your module's `build.gradle` file, add the following dependency:

```gradle
dependencies {
    implementation 'com.github.padmakar1811:AppSecureGuard:v1.0.1'
}
```

### Step 3: Sync the Project

Click **Sync Now** in Android Studio to download and integrate the library into your project.

## Usage

To use **AppProtectGuard**, you can call the `isDeviceRootedOrEmulator` function to check for any security threats or unauthorized environments (rooted device, emulator, debugging, etc.).

### Example

```kotlin
import com.locateme.utils.AppProtectGuardApp

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

       if (AppProtectGuard.nativeShouldShowSecurityAlert()) {
            Log.e("Security", "Risky environment")
        }
    }
}
```

## How It Works

The library checks for multiple security risks:

### 1. **Root Detection**: 
It uses different methods to check if the device has been rooted, such as checking for common root paths or the presence of the `su` binary.

### 2. **Emulator Detection**: 
It verifies if the device is an emulator by checking the device's fingerprint, model, manufacturer, and hardware.

### 3. **Debugger Detection**: 
It checks if a debugger is attached to the application using `Debug.isDebuggerConnected()` and other system properties.

### 4. **Frida Injection Detection**: 
It attempts to detect if the Frida toolkit is running by looking for processes related to Frida.

### 5. **VPN Detection**: 
It checks if the device is connected to a VPN by inspecting the system’s connectivity manager.

### 6. **Proxy Detection**: 
It detects if the HTTP proxy is enabled by checking system properties for a proxy host and port.

### 7. **Developer Options Detection**: 
It checks if the device's developer options are enabled, which could signal potential risks in terms of security.
