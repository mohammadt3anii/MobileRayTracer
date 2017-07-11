package puscas.mobilertapp;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.NumberPicker;

import java.io.File;
import java.io.FileFilter;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.regex.Pattern;

public final class MainActivity extends Activity {
    static {
        System.loadLibrary("MobileRT");
        System.loadLibrary("Components");
        System.loadLibrary("DrawView");
    }

    private DrawView drawView_;
    private GLDrawView glDrawView_;
    private NumberPicker pickerScene_;
    private NumberPicker pickerShader_;
    private NumberPicker pickerThreads_;
    private NumberPicker pickerSampler_;
    private NumberPicker pickerSamplesPixel_;
    private NumberPicker pickerSamplesLight_;

    private static int getNumCoresOldPhones() {
        try {
            final File dir = new File("/sys/devices/system/cpu/");
            final File[] files = dir.listFiles(new CpuFilter());
            return files.length;
        } catch (final RuntimeException ignored) {
            return 1;
        }
    }

    private static int getNumberOfCores() {
        return (Build.VERSION.SDK_INT < 17) ? MainActivity.getNumCoresOldPhones() :
                Runtime.getRuntime().availableProcessors();
    }

    @Override
    protected void onPause() {
        super.onPause();
        glDrawView_.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        glDrawView_.onResume();
    }

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        try {
            setContentView(R.layout.activity_main);
        } catch (final RuntimeException e) {
            e.fillInStackTrace();
        }

        final ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        final ConfigurationInfo info = am.getDeviceConfigurationInfo();
        final boolean supportES2 = (info.reqGlEsVersion >= 0x20000);
        if (supportES2) {
            final MainRenderer mainRenderer = new MainRenderer();
            glDrawView_ = (GLDrawView) findViewById(R.id.drawLayout);
            glDrawView_.setEGLContextClientVersion(2);
            glDrawView_.setEGLConfigChooser(8, 8, 8, 8, 0, 0);
            glDrawView_.setRenderer(mainRenderer);
            glDrawView_.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

            final ScheduledExecutorService scheduler_ = Executors.newSingleThreadScheduledExecutor();
            scheduler_.scheduleAtFixedRate(() -> glDrawView_.requestRender(), 0L, 200, TimeUnit.MILLISECONDS);
        } else {
            Log.e("OpenGLES 2", "Your device doesn't support ES 2. (" + info.reqGlEsVersion + ')');
        }

        /*final TextView textView_ = (TextView) findViewById(R.id.timeText);
        drawView_ = (DrawView) findViewById(R.id.drawLayout);
        drawView_.setVisibility(View.VISIBLE);
        drawView_.setView(textView_);
        drawView_.buttonRender_ = (Button) findViewById(R.id.renderButton);*/

        final String[] scenes = {"Cornell", "Spheres", "Spheres2", "Cornell2"};
        pickerScene_ = (NumberPicker) findViewById(R.id.pickerScene);
        pickerScene_.setMinValue(0);
        pickerScene_.setMaxValue(scenes.length - 1);
        pickerScene_.setDisplayedValues(scenes);
        pickerScene_.setWrapSelectorWheel(true);
        pickerScene_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);

        final String[] shaders = {"NoShadows", "Whitted", "PathTracer", ""};
        pickerShader_ = (NumberPicker) findViewById(R.id.pickerShader);
        pickerShader_.setMinValue(0);
        pickerShader_.setMaxValue(shaders.length - 1);
        pickerShader_.setDisplayedValues(shaders);
        pickerShader_.setWrapSelectorWheel(true);
        pickerShader_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);

        final int maxSamplesPixel = 10;
        final String[] samplesPixel = new String[maxSamplesPixel];
        for (int i = 0; i < maxSamplesPixel; i++) {
            samplesPixel[i] = Integer.toString((i + 1) * (i + 1));
        }
        pickerSamplesPixel_ = (NumberPicker) findViewById(R.id.pickerSamplesPixel);
        pickerSamplesPixel_.setMinValue(1);
        pickerSamplesPixel_.setMaxValue(maxSamplesPixel);
        pickerSamplesPixel_.setDisplayedValues(samplesPixel);
        pickerSamplesPixel_.setWrapSelectorWheel(true);
        pickerSamplesPixel_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);

        final int maxSamplesLight = 100;
        final String[] samplesLight;
        try {
            samplesLight = new String[maxSamplesLight];
        } catch (final OutOfMemoryError e) {
            e.fillInStackTrace();
            throw e;
        }
        for (int i = 0; i < maxSamplesLight; i++) {
            samplesLight[i] = Integer.toString(i + 1);
        }
        pickerSamplesLight_ = (NumberPicker) findViewById(R.id.pickerSamplesLight);
        pickerSamplesLight_.setMinValue(1);
        pickerSamplesLight_.setMaxValue(maxSamplesLight);
        pickerSamplesLight_.setDisplayedValues(samplesLight);
        pickerSamplesLight_.setWrapSelectorWheel(true);
        pickerSamplesLight_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);

        final String[] samplers = {"Stratified", "HaltonSeq"};
        pickerSampler_ = (NumberPicker) findViewById(R.id.pickerSampler);
        pickerSampler_.setMinValue(0);
        pickerSampler_.setMaxValue(samplers.length - 1);
        pickerSampler_.setDisplayedValues(samplers);
        pickerSampler_.setWrapSelectorWheel(true);
        pickerSampler_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);

        pickerThreads_ = (NumberPicker) findViewById(R.id.pickerThreads);
        pickerThreads_.setMinValue(1);
        pickerThreads_.setMaxValue(MainActivity.getNumberOfCores() << 1);
        pickerThreads_.setWrapSelectorWheel(true);
        pickerThreads_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
    }

    public void startRender(final View view) {
        if (view == null) {
            return;
        }
        switch (drawView_.isWorking()) {
            case 0://if ray-tracer is idle
                drawView_.createScene(
                        pickerScene_.getValue(),
                        pickerShader_.getValue(),
                        pickerThreads_.getValue(),
                        pickerSampler_.getValue(),
                        Integer.parseInt(pickerSamplesPixel_.getDisplayedValues()
                                [pickerSamplesPixel_.getValue() - 1]
                        ),
                        Integer.parseInt(
                                pickerSamplesLight_.getDisplayedValues()
                                        [pickerSamplesLight_.getValue() - 1]
                        )
                );
                drawView_.startRender();
                break;

            default:
                this.drawView_.stopDrawing();//if ray-tracer is busy
                break;
        }
    }

    private static final class CpuFilter implements FileFilter {
        CpuFilter() {
            super();
        }

        @Override
        public final boolean accept(final File file) {
            return Pattern.matches("cpu[0-9]+", file.getName());
        }
    }
}
