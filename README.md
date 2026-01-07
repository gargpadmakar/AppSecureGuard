---

# AppProtectGuard

**AppProtectGuard** is a security library designed for Android applications to help detect various types of vulnerabilities and unauthorized modifications. It can detect rooted devices, emulators, active debuggers, VPN usage, proxy settings, and more. The library ensures that your app runs in a secure environment and provides protection against tampering.

## Features

- **Root Detection**: Detects if the device is rooted using multiple methods.
- **Emulator Detection**: Identifies whether the device is running on an emulator.
- **Debugger Detection**: Checks if a debugger is attached to the app.
- **Frida Injection Detection**: Detects the presence of Frida (a dynamic instrumentation toolkit).
- **VPN Detection**: Detects if the device is connected to a VPN.
- **Proxy Detection**: Detects the usage of HTTP proxy.
- **Developer Options Detection**: Identifies if developer options or USB debugging are enabled.
-  **Native (JNI) Security Checks**  
  Critical security checks are implemented in native C/C++ code to improve resistance against reverse engineering and runtime bypass.


## Installation

To include **AppProtectGuard** in your project, follow the steps below:

### Step 1: Add JitPack Repository

In your `root build.gradle` file, add the JitPack repository:

```gradle
allprojects {
    repositories {
        maven { url 'https://jitpack.io' }
    }
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
