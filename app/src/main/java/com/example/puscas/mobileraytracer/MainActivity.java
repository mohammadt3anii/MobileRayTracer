package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;
import android.widget.TextView;

import java.io.File;
import java.io.FileFilter;
import java.util.regex.Pattern;

/**
 * Created by Tiago on 09/10/2016.
 */

public class MainActivity extends Activity
{
    static {
        System.loadLibrary("DrawView");
    }

    private Button mRenderButton_;
    private TextView textView_;
    private DrawView drawView_;
    private NumberPicker pickerScene_;
    private NumberPicker pickerShader_;
    private NumberPicker pickerThreads_;
    private NumberPicker pickerSampler_;
    private NumberPicker pickerSamples_;

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

        mRenderButton_ = (Button) findViewById(R.id.renderButton);
        textView_ = (TextView) findViewById(R.id.timeText);
        drawView_ = (DrawView) findViewById(R.id.viewDraw);
        drawView_.setHandler(new MessageHandler());
        drawView_.setVisibility(View.INVISIBLE);

        String[] scenes = {"Cornell", "Spheres"};
        pickerScene_ = (NumberPicker) findViewById(R.id.pickerScene);
        pickerScene_.setMinValue(0);
        pickerScene_.setMaxValue(scenes.length - 1);
        pickerScene_.setDisplayedValues(scenes);
        pickerScene_.setWrapSelectorWheel(true);
        pickerScene_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        String[] shaders = {"NoShadows", "Whitted"};
        pickerShader_ = (NumberPicker) findViewById(R.id.pickerShader);
        pickerShader_.setMinValue(0);
        pickerShader_.setMaxValue(shaders.length - 1);
        pickerShader_.setDisplayedValues(shaders);
        pickerShader_.setWrapSelectorWheel(true);
        pickerShader_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        String[] samplers = {"Stratified", "Jittered"};
        pickerSampler_ = (NumberPicker) findViewById(R.id.pickerSampler);
        pickerSampler_.setMinValue(0);
        pickerSampler_.setMaxValue(samplers.length - 1);
        pickerSampler_.setDisplayedValues(samplers);
        pickerSampler_.setWrapSelectorWheel(true);
        pickerSampler_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        pickerSamples_ = (NumberPicker) findViewById(R.id.pickerSamples);
        pickerSamples_.setMinValue(1);
        pickerSamples_.setMaxValue(16);
        pickerSamples_.setWrapSelectorWheel(true);
        pickerSamples_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        pickerThreads_ = (NumberPicker) findViewById(R.id.pickerThreads);
        pickerThreads_.setMinValue(1);
        pickerThreads_.setMaxValue(getNumberOfCores() * 2);
        pickerThreads_.setWrapSelectorWheel(true);
        pickerThreads_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);
    }

    public void startRender(View view)
    {
        mRenderButton_.setEnabled(false);
        drawView_.createScene(pickerScene_.getValue(), pickerShader_.getValue(),
                pickerThreads_.getValue(), textView_, pickerSampler_.getValue(),
                pickerSamples_.getValue());
        drawView_.setVisibility(View.VISIBLE);
        drawView_.invalidate();
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