package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Color;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.NumberPicker;

/**
 * Created by Tiago on 22/11/2016.
 */

public class CustomNumberPicker extends NumberPicker {

    public CustomNumberPicker(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void addView(View child) {
        super.addView(child);
        this.updateView(child);
    }

    @Override
    public void addView(View child, int index, ViewGroup.LayoutParams params) {
        super.addView(child, index, params);
        this.updateView(child);
    }

    @Override
    public void addView(View child, ViewGroup.LayoutParams params) {
        super.addView(child, params);
        this.updateView(child);
    }

    private void updateView(View view) {
        if (view instanceof EditText) {
            ((EditText) view).setTextSize(15);
            ((EditText) view).setTextColor(Color.parseColor("#333333"));
        }
    }

}