package com.padmakar.secureguard

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import com.google.android.material.bottomsheet.BottomSheetDialogFragment

internal class SecurityAlertBottomSheet : BottomSheetDialogFragment() {

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        val view = inflater.inflate(
            R.layout.bottomsheet_security_alert,
            container,
            false
        )

        val tvMessage = view.findViewById<TextView>(R.id.tvMessage)
        val btnOk = view.findViewById<Button>(R.id.btnOk)

        val issues = mutableListOf<String>()

        // 🔍 Individual signals
        val isHook = AppProtectGuard.isHookDetectedInternal()
        val isNetwork = AppProtectGuard.isNetworkBlocked()
        val isGenericRisk = AppProtectGuard.shouldShowSecurityAlert()

        /*
         * ✅ SHOW ONLY RELEVANT ISSUES
         */

        // 1️⃣ Hooking / instrumentation
        if (isHook) {
            issues.add("• Debugger / Frida / hooking framework detected")
        }

        // 2️⃣ Network / MITM
        if (isNetwork) {
            issues.add("• Proxy / VPN / MITM network configuration detected")
        }

        // 3️⃣ Device integrity (ONLY if no hook & no network)
        if (issues.isEmpty() && isGenericRisk) {
            issues.add("• Device integrity issue detected (root / emulator / modified OS)")
        }

        // 🛡️ Safety fallback
        if (issues.isEmpty()) {
            issues.add("• Suspicious environment detected")
        }

        // 📝 UI
        tvMessage.text = buildString {
            append("Security risks detected on this device:\n\n")
            issues.forEach { append(it).append("\n") }
            append("\nSome features may be limited to protect your data.")
        }

        btnOk.setOnClickListener { dismiss() }
        isCancelable = false

        return view
    }
}
