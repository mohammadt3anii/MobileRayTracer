package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;
import android.widget.TextView;

import java.io.File;
import java.io.FileFilter;
import java.util.regex.Pattern;

public class MainActivity extends Activity
{
    static {
        System.loadLibrary("DrawView");
    }

    private TextView textView_;
    private DrawView drawView_;
    private NumberPicker pickerScene_;
    private NumberPicker pickerShader_;
    private NumberPicker pickerThreads_;
    private NumberPicker pickerSampler_;
    private NumberPicker pickerSamples_;

    private int getNumCoresOldPhones() {
        final class CpuFilter implements FileFilter {
            public boolean accept(File pathname) {
                return Pattern.matches("cpu[0-9]+", pathname.getName());
            }
        }
        try {
            final File dir = new File("/sys/devices/system/cpu/");
            final File[] files = dir.listFiles(new CpuFilter());
            return files.length;
        } catch (Exception e) {
            return 1;
        }
    }

    private int getNumberOfCores() {
        if (Build.VERSION.SDK_INT < 17) {
            return getNumCoresOldPhones();
        } else {
            return Runtime.getRuntime().availableProcessors();
        }
    }

    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView_ = (TextView) findViewById(R.id.timeText);
        drawView_ = (DrawView) findViewById(R.id.viewDraw);
        drawView_.setVisibility(View.INVISIBLE);
        drawView_.setButton((Button) findViewById(R.id.renderButton));

        final String[] scenes = {"Cornell", "Spheres"};
        pickerScene_ = (NumberPicker) findViewById(R.id.pickerScene);
        pickerScene_.setMinValue(0);
        pickerScene_.setMaxValue(scenes.length - 1);
        pickerScene_.setDisplayedValues(scenes);
        pickerScene_.setWrapSelectorWheel(true);
        pickerScene_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        final String[] shaders = {"NoShadows", "Whitted"};
        pickerShader_ = (NumberPicker) findViewById(R.id.pickerShader);
        pickerShader_.setMinValue(0);
        pickerShader_.setMaxValue(shaders.length - 1);
        pickerShader_.setDisplayedValues(shaders);
        pickerShader_.setWrapSelectorWheel(true);
        pickerShader_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        final int maxSamples = 12;
        final String[] samples = new String[maxSamples];
        for (int i = 0; i < maxSamples; i++) {
            samples[i] = Integer.toString((i + 1) * (i + 1));
        }
        pickerSamples_ = (NumberPicker) findViewById(R.id.pickerSamples);
        pickerSamples_.setMinValue(1);
        pickerSamples_.setMaxValue(maxSamples);
        pickerSamples_.setDisplayedValues(samples);
        pickerSamples_.setWrapSelectorWheel(true);
        pickerSamples_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        final String[] samplers = {"Stratified", "Jittered"};
        pickerSampler_ = (NumberPicker) findViewById(R.id.pickerSampler);
        pickerSampler_.setMinValue(0);
        pickerSampler_.setMaxValue(samplers.length - 1);
        pickerSampler_.setDisplayedValues(samplers);
        pickerSampler_.setWrapSelectorWheel(true);
        pickerSampler_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);

        pickerThreads_ = (NumberPicker) findViewById(R.id.pickerThreads);
        pickerThreads_.setMinValue(1);
        pickerThreads_.setMaxValue(getNumberOfCores() * 2);
        pickerThreads_.setWrapSelectorWheel(true);
        pickerThreads_.setDescendantFocusability(NumberPicker.FOCUS_BLOCK_DESCENDANTS);
    }

    final public void startRender(View view)
    {
        switch (drawView_.isWorking()) {
            case 0://if ray-tracer is idle
                drawView_.createScene(
                        pickerScene_.getValue(),
                        pickerShader_.getValue(),
                        pickerThreads_.getValue(),
                        textView_,
                        pickerSampler_.getValue(),
                        Integer.parseInt(
                                pickerSamples_.getDisplayedValues()
                                        [pickerSamples_.getValue() - 1]
                        )
                );
                drawView_.setVisibility(View.VISIBLE);
                drawView_.startRender(67);
                break;

            case 1://if ray-tracer is busy
                drawView_.stopRender();
                break;

            default:
                break;
        }
    }
}