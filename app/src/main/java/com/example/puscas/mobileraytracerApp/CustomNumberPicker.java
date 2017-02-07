package com.example.puscas.mobileraytracerApp;

import android.content.Context;
import android.graphics.Color;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.NumberPicker;

public class CustomNumberPicker extends NumberPicker {
    public CustomNumberPicker(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void addView(View child, ViewGroup.LayoutParams params) {
        super.addView(child, params);
        ((EditText) child).setTextSize(15);
        ((EditText) child).setTextColor(Color.parseColor("#000000"));
    }
}