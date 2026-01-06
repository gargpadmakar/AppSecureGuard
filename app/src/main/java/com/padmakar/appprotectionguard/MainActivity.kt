package com.padmakar.appprotectionguard

import android.os.Bundle
import android.util.Log
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import com.padmakar.secureguard.AppProtectGuard


class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }
        // ⚠️ Optional soft checks
        if (AppProtectGuard.nativeShouldShowSecurityAlert()) {
            Log.e("Security", "Risky environment")
        }

        if (AppProtectGuard.nativeIsHookDetected()) {
            Log.e("Security", "Hook detected")
        }
    }
}