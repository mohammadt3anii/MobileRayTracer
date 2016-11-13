package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

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
    private MainActivity.MessageHandler handler_;
    private NumberPicker numThreads_;
    private RadioGroup groupScene_;
    private RadioGroup groupShader_;

    /**
     * Gets the number of cores available in this device, across all processors.
     * Requires: Ability to peruse the filesystem at "/sys/devices/system/cpu"
     *
     * @return The number of cores, or 1 if failed to get result
     */
    private int getNumCoresOldPhones() {
        //Private Class to display only CPU devices in the directory listing
        class CpuFilter implements FileFilter {
            @Override
            public boolean accept(File pathname) {
                //Check if filename is "cpu", followed by a single digit number
                return Pattern.matches("cpu[0-9]+", pathname.getName());
            }
        }

        try {
            //Get directory containing CPU info
            File dir = new File("/sys/devices/system/cpu/");
            //Filter to only list the devices we care about
            File[] files = dir.listFiles(new CpuFilter());
            //Return the number of cores (virtual CPU devices)
            return files.length;
        } catch (Exception e) {
            //Default to return 1 core
            return 1;
        }
    }

    private int getNumberOfCores() {
        if (Build.VERSION.SDK_INT >= 17) {
            return Runtime.getRuntime().availableProcessors();
        } else {
            return getNumCoresOldPhones();
        }
    }


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView_ = (TextView) findViewById(R.id.timeText);
        drawView_ = (DrawView) findViewById(R.id.viewDraw);
        handler_ = new MainActivity.MessageHandler();
        drawView_.setHandler(handler_);
        mRenderButton_ = (Button) findViewById(R.id.renderButton);
        drawView_.setVisibility(View.INVISIBLE);
        numThreads_ = (NumberPicker) findViewById(R.id.pickerThreads);
        numThreads_.setMaxValue(getNumberOfCores() * 2);
        numThreads_.setMinValue(1);
        numThreads_.setWrapSelectorWheel(true);
        numThreads_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        groupScene_ = (RadioGroup) findViewById(R.id.radioGroupScene);
        groupShader_ = (RadioGroup) findViewById(R.id.radioGroupShader);

        this.mRenderButton_.setEnabled(false);
    }

    public void startRender(View view) {
        mRenderButton_.setEnabled(false);
        drawView_.createScene(scene_, shader_, numThreads_.getValue(), textView_);
        drawView_.invalidate();
        drawView_.setVisibility(View.VISIBLE);
    }

    public void onSceneRadioButtonClicked(View view)
    {
        // Is the button now checked?
        if (((RadioButton) view).isChecked() == false) return;

        // Check which radio button was clicked
        switch (view.getId()) {
            case R.id.radioCornell:
                this.scene_ = 0; // cornell
                break;

            case R.id.radioSpheres:
                this.scene_ = 1; // spheres
                break;

            default:
                break;
        }

        if (groupShader_.getCheckedRadioButtonId() != -1) {
            this.mRenderButton_.setEnabled(true);
        }
    }

    public void onShaderRadioButtonClicked(View view)
    {
        // Is the button now checked?
        if (((RadioButton) view).isChecked() == false) return;

        // Check which radio button was clicked
        switch (view.getId()) {
            case R.id.radioNoShadows:
                this.shader_ = 0; // No Shadows
                break;

            case R.id.radioWhitted:
                this.shader_ = 1; // Whitted
                break;

            default:
                break;
        }
        if (groupScene_.getCheckedRadioButtonId() != -1) {
            this.mRenderButton_.setEnabled(true);
        }

    }

    private class MessageHandler extends Handler {
        @Override
        public void handleMessage(Message inputMessage) {
            switch (inputMessage.what) {
                case 1:        // Render finished
                    MainActivity.this.mRenderButton_.setEnabled(true);
                    break;

                default:
                    super.handleMessage(inputMessage);
                    break;
            }
        }
    }
}