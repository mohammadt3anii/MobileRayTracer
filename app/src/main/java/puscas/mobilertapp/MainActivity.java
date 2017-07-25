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
import android.widget.Button;
import android.widget.NumberPicker;
import android.widget.TextView;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.io.InputStream;
import java.util.regex.Pattern;

public final class MainActivity extends Activity {

    static {
        System.loadLibrary("MobileRT");
        System.loadLibrary("Components");
        System.loadLibrary("DrawView");
    }

    private DrawView drawView_;
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

    private String loadAsset(final String filename) {
        String asset = null;
        InputStream stream = null;
        try {
            stream = getAssets().open(filename);
            final int size = stream.available();
            final byte[] buffer = new byte[size];
            stream.read(buffer);
            asset = new String(buffer);
        } catch (final IOException e) {
            Log.e("Assets", "Couldn't read asset " + filename);
            Log.e("Assets", e.getMessage());
        } finally {
            if (stream != null) {
                try {
                    stream.close();
                } catch (final IOException e) {
                    Log.e("Assets", "Couldn't close asset " + filename);
                    Log.e("Assets", e.toString());
                }
            }
        }
        return asset;
    }

    @Override
    protected void onPause() {
        super.onPause();
        drawView_.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        drawView_.onResume();
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
            drawView_ = (DrawView) findViewById(R.id.drawLayout);
            if (drawView_ == null) {
                Log.e("DrawView", "DrawView is NULL !!!");
                System.exit(0);
            }
            drawView_.setVisibility(View.INVISIBLE);
            drawView_.setEGLContextClientVersion(2);
            drawView_.setEGLConfigChooser(8, 8, 8, 8, 0, 0);
            final MainRenderer renderer = new MainRenderer();
            drawView_.renderer = renderer;
            drawView_.setRenderer(renderer);
            drawView_.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
            final String vertexShader = loadAsset("VertexShader.glsl");
            final String fragmentShader = loadAsset("FragmentShader.glsl");
            drawView_.renderer.vertexShaderCode = vertexShader;
            drawView_.renderer.fragmentShaderCode = fragmentShader;

            drawView_.buttonRender_ = (Button) findViewById(R.id.renderButton);
            if (drawView_.buttonRender_ == null) {
                Log.e("Button", "Button is NULL !!!");
                System.exit(0);
            }
            final TextView textView = (TextView) findViewById(R.id.timeText);
            if (textView == null) {
                Log.e("TextView", "TextView is NULL !!!");
                System.exit(0);
            }
            drawView_.setView(textView);
            drawView_.buttonRender_ = (Button) findViewById(R.id.renderButton);
            if (drawView_.buttonRender_ == null) {
                Log.e("Button", "Button is NULL !!!");
                System.exit(0);
            }
        } else {
            Log.e("OpenGLES 2", "Your device doesn't support ES 2. (" + info.reqGlEsVersion + ')');
            System.exit(0);
        }

        final String[] scenes = {"Cornell", "Spheres", "Spheres2", "Cornell2"};
        pickerScene_ = (NumberPicker) findViewById(R.id.pickerScene);
        if (pickerScene_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
        pickerScene_.setMinValue(0);
        pickerScene_.setMaxValue(scenes.length - 1);
        pickerScene_.setDisplayedValues(scenes);
        pickerScene_.setWrapSelectorWheel(true);
        pickerScene_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);

        final String[] shaders = {"NoShadows", "Whitted", "PathTracer", ""};
        pickerShader_ = (NumberPicker) findViewById(R.id.pickerShader);
        if (pickerShader_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
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
        if (pickerSamplesPixel_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
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
        if (pickerSamplesLight_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
        pickerSamplesLight_.setMinValue(1);
        pickerSamplesLight_.setMaxValue(maxSamplesLight);
        pickerSamplesLight_.setDisplayedValues(samplesLight);
        pickerSamplesLight_.setWrapSelectorWheel(true);
        pickerSamplesLight_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);

        final String[] samplers = {"Stratified", "HaltonSeq"};
        pickerSampler_ = (NumberPicker) findViewById(R.id.pickerSampler);
        if (pickerSampler_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
        pickerSampler_.setMinValue(0);
        pickerSampler_.setMaxValue(samplers.length - 1);
        pickerSampler_.setDisplayedValues(samplers);
        pickerSampler_.setWrapSelectorWheel(true);
        pickerSampler_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);

        pickerThreads_ = (NumberPicker) findViewById(R.id.pickerThreads);
        if (pickerThreads_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
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

            default://if ray-tracer is busy
                this.drawView_.stopDrawing();
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
