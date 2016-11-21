package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Build.VERSION;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import com.example.puscas.mobileraytracer.R.id;
import com.example.puscas.mobileraytracer.R.layout;

import java.io.File;
import java.io.FileFilter;
import java.util.regex.Pattern;

public class MainActivity extends Activity
{
    static {
        System.loadLibrary("DrawView");
    }

    private Button mRenderButton_;
    private TextView textView_;
    private DrawView drawView_;
    private int scene_;
    private int shader_;
    private MessageHandler handler_;
    private NumberPicker numThreads_;
    private RadioGroup groupScene_;
    private RadioGroup groupShader_;

    private int getNumCoresOldPhones() {
        class CpuFilter implements FileFilter {
            @Override
            public boolean accept(File pathname) {
                return Pattern.matches("cpu[0-9]+", pathname.getName());
            }
        }
        try {
            File dir = new File("/sys/devices/system/cpu/");
            File[] files = dir.listFiles(new CpuFilter());
            return files.length;
        } catch (Exception e) {
            return 1;
        }
    }

    private int getNumberOfCores() {
        if (VERSION.SDK_INT >= 17) {
            return Runtime.getRuntime().availableProcessors();
        } else {
            return this.getNumCoresOldPhones();
        }
    }


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        this.setContentView(layout.activity_main);

        this.textView_ = (TextView) this.findViewById(id.timeText);
        this.drawView_ = (DrawView) this.findViewById(id.viewDraw);
        this.handler_ = new MessageHandler();
        this.drawView_.setHandler(this.handler_);
        this.mRenderButton_ = (Button) this.findViewById(id.renderButton);
        this.drawView_.setVisibility(View.INVISIBLE);
        this.numThreads_ = (NumberPicker) this.findViewById(id.pickerThreads);
        this.numThreads_.setMinValue(1);
        this.numThreads_.setMaxValue(this.getNumberOfCores() * 2);
        this.numThreads_.setDisplayedValues(new String[]{"One", "Two", "Three", "Four", "Five"});
        this.numThreads_.setWrapSelectorWheel(true);
        this.numThreads_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        this.groupScene_ = (RadioGroup) this.findViewById(id.radioGroupScene);
        this.groupShader_ = (RadioGroup) this.findViewById(id.radioGroupShader);

        mRenderButton_.setEnabled(false);
    }

    public void startRender(View view) {
        this.mRenderButton_.setEnabled(false);
        this.drawView_.createScene(this.scene_, this.shader_, this.numThreads_.getValue(), this.textView_);
        this.drawView_.setVisibility(View.VISIBLE);
        this.drawView_.invalidate();
    }

    public void onSceneRadioButtonClicked(View view)
    {
        // Is the button now checked?
        if (((RadioButton) view).isChecked() == false) return;

        // Check which radio button was clicked
        switch (view.getId()) {
            case id.radioCornell:
                scene_ = 0; // cornell
                break;

            case id.radioSpheres:
                scene_ = 1; // spheres
                break;

            default:
                break;
        }

        if (this.groupShader_.getCheckedRadioButtonId() != -1) {
            mRenderButton_.setEnabled(true);
        }
    }

    public void onShaderRadioButtonClicked(View view)
    {
        // Is the button now checked?
        if (((RadioButton) view).isChecked() == false) return;

        // Check which radio button was clicked
        switch (view.getId()) {
            case id.radioNoShadows:
                shader_ = 0; // No Shadows
                break;

            case id.radioWhitted:
                shader_ = 1; // Whitted
                break;

            default:
                break;
        }
        if (this.groupScene_.getCheckedRadioButtonId() != -1) {
            mRenderButton_.setEnabled(true);
        }

    }

    private class MessageHandler extends Handler {
        @Override
        public void handleMessage(Message inputMessage) {
            switch (inputMessage.what) {
                case 1:        // Render finished
                    mRenderButton_.setEnabled(true);
                    break;

                default:
                    super.handleMessage(inputMessage);
                    break;
            }
        }
    }
}