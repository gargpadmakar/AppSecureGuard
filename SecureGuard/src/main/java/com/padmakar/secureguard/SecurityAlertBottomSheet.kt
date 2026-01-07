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

        val header = AppProtectGuard.getSecurityHeader()
        val issues = AppProtectGuard.getRiskMessages()
        val footer = AppProtectGuard.getSecurityFooter()

        tvMessage.text = buildString {
            append(header)
            issues.forEach { append(it).append("\n") }
            append(footer)
        }

        btnOk.setOnClickListener {
            AppProtectGuard.forceExit()
        }

        isCancelable = false
        return view
    }
}
