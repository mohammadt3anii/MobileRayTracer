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
        this.numThreads_.setMaxValue(this.getNumberOfCores() * 2);
        this.numThreads_.setMinValue(1);
        this.numThreads_.setWrapSelectorWheel(true);
        this.numThreads_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);
    }

    public void startRender(View view) {
        this.mRenderButton_.setEnabled(false);
        this.drawView_.createScene(this.scene_, this.shader_, this.numThreads_.getValue(), this.textView_);
        this.drawView_.invalidate();
        this.drawView_.setVisibility(View.VISIBLE);
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