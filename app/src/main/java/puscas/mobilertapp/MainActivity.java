package puscas.mobilertapp;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.NumberPicker;
import android.widget.TextView;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.io.InputStream;
import java.util.Locale;
import java.util.regex.Pattern;

public final class MainActivity extends Activity {

    static {
        System.loadLibrary("MobileRT");
        System.loadLibrary("Components");
        System.loadLibrary("AppInterface");
    }

    private DrawView drawView_;
    private NumberPicker pickerScene_;
    private NumberPicker pickerShader_;
    private NumberPicker pickerThreads_;
    private NumberPicker pickerAccelerator_;
    private NumberPicker pickerSamplesPixel_;
    private NumberPicker pickerSamplesLight_;
    private NumberPicker pickerSizes_;
    private String objText_;
    private String matText_;

    private static int getNumCoresOldPhones() {
        try {
            final File dir = new File("/sys/devices/system/cpu/");
            final File[] files = dir.listFiles(new CpuFilter());
            return files.length;
        } catch (final RuntimeException ignored) {
            Log.e("getNumCoresOldPhones", "Can't get number of cores available!!!");
            return 1;
        }
    }

    private static int getNumberOfCores() {
        return (Build.VERSION.SDK_INT < 17) ? MainActivity.getNumCoresOldPhones() :
                Runtime.getRuntime().availableProcessors();
    }

    @Override
    public void onSaveInstanceState(final Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        savedInstanceState.putInt("pickerScene", pickerScene_.getValue());
        savedInstanceState.putInt("pickerShader", pickerShader_.getValue());
        savedInstanceState.putInt("pickerThreads", pickerThreads_.getValue());
        savedInstanceState.putInt("pickerAccelerator", pickerAccelerator_.getValue());
        savedInstanceState.putInt("pickerSamplesPixel", pickerSamplesPixel_.getValue());
        savedInstanceState.putInt("pickerSamplesLight", pickerSamplesLight_.getValue());
        savedInstanceState.putInt("pickerSizes", pickerSizes_.getValue());
    }

    private String readTextAsset(final String filename) {
        final AssetManager am = getAssets();
        String asset = null;
        try (final InputStream stream = am.open(filename)) {
            final int size = stream.available();
            final byte[] buffer = new byte[size];
            final int bytes = stream.read(buffer);
            if (bytes <= 0) {
                asset = null;
            } else {
                asset = new String(buffer);
            }
        } catch (final OutOfMemoryError e1) {
            Log.e("Assets", "Not enough memory for asset  " + filename);
            Log.e("Assets", e1.getMessage());
            throw e1;
        } catch (final IOException e2) {
            Log.e("Assets", "Couldn't read asset " + filename);
            Log.e("Assets", e2.getMessage());
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

        int pickerScene = 0;
        int pickerShader = 0;
        int pickerThreads = MainActivity.getNumberOfCores();
        int pickerAccelerator = 0;
        int pickerSamplesPixel = 1;
        int pickerSamplesLight = 1;
        int pickerSizes = 0;
        if (savedInstanceState != null) {
            pickerScene = savedInstanceState.getInt("pickerScene");
            pickerShader = savedInstanceState.getInt("pickerShader");
            pickerThreads = savedInstanceState.getInt("pickerThreads");
            pickerAccelerator = savedInstanceState.getInt("pickerAccelerator");
            pickerSamplesPixel = savedInstanceState.getInt("pickerSamplesPixel");
            pickerSamplesLight = savedInstanceState.getInt("pickerSamplesLight");
            pickerSizes = savedInstanceState.getInt("pickerSizes");
        }

        try {
            setContentView(R.layout.activity_main);
        } catch (final RuntimeException e) {
            e.fillInStackTrace();
        }

        final ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        assert am != null;
        final ConfigurationInfo info = am.getDeviceConfigurationInfo();
        final boolean supportES2 = (info.reqGlEsVersion >= 0x20000);
        if (supportES2) {
            drawView_ = findViewById(R.id.drawLayout);
            if (drawView_ == null) {
                Log.e("DrawView", "DrawView is NULL !!!");
                System.exit(0);
            }
            drawView_.setVisibility(View.INVISIBLE);
            drawView_.setEGLContextClientVersion(2);
            drawView_.setEGLConfigChooser(8, 8, 8, 8, 0, 0);
            final MainRenderer renderer = new MainRenderer();
            drawView_.renderer_ = renderer;
            drawView_.setRenderer(renderer);
            drawView_.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
            final String vertexShader = readTextAsset("Shaders/VertexShader.glsl");
            final String fragmentShader = readTextAsset("Shaders/FragmentShader.glsl");
            drawView_.renderer_.vertexShaderCode = vertexShader;
            drawView_.renderer_.fragmentShaderCode = fragmentShader;

            drawView_.viewText_.buttonRender_ = findViewById(R.id.renderButton);
            drawView_.viewText_.buttonRender_.setOnLongClickListener((View v) -> {
                this.recreate();
                return false;
            });
            if (drawView_.viewText_.buttonRender_ == null) {
                Log.e("Button", "Button is NULL !!!");
                System.exit(0);
            }
            final TextView textView = findViewById(R.id.timeText);
            if (textView == null) {
                Log.e("ViewText", "ViewText is NULL !!!");
                System.exit(0);
            }
            drawView_.setView(textView);
            drawView_.viewText_.buttonRender_ = findViewById(R.id.renderButton);
            if (drawView_.viewText_.buttonRender_ == null) {
                Log.e("Button", "Button is NULL !!!");
                System.exit(0);
            }
        } else {
            Log.e("OpenGLES 2", "Your device doesn't support ES 2. (" + info.reqGlEsVersion + ')');
            System.exit(0);
        }

        pickerScene_ = findViewById(R.id.pickerScene);
        if (pickerScene_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }

        pickerScene_.setMinValue(0);
        final String[] scenes = {"Cornell", "Spheres", "Cornell2", "Spheres2", "OBJ"};
        pickerScene_.setMaxValue(scenes.length - 1);
        pickerScene_.setDisplayedValues(scenes);
        pickerScene_.setWrapSelectorWheel(true);
        pickerScene_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerScene_.setValue(pickerScene);

        pickerShader_ = findViewById(R.id.pickerShader);
        if (pickerShader_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }

        pickerShader_.setMinValue(0);
        final String[] shaders = {"NoShadows", "Whitted", "PathTracer", "DepthMap", "Diffuse"};
        pickerShader_.setMaxValue(shaders.length - 1);
        pickerShader_.setDisplayedValues(shaders);
        pickerShader_.setWrapSelectorWheel(true);
        pickerShader_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerShader_.setValue(pickerShader);

        final int maxSamplesPixel = 10;
        final String[] samplesPixel = new String[maxSamplesPixel];
        for (int i = 0; i < maxSamplesPixel; i++) {
            samplesPixel[i] = Integer.toString((i + 1) * (i + 1));
        }
        pickerSamplesPixel_ = findViewById(R.id.pickerSamplesPixel);
        if (pickerSamplesPixel_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
        pickerSamplesPixel_.setMinValue(1);
        pickerSamplesPixel_.setMaxValue(maxSamplesPixel);
        pickerSamplesPixel_.setDisplayedValues(samplesPixel);
        pickerSamplesPixel_.setWrapSelectorWheel(true);
        pickerSamplesPixel_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerSamplesPixel_.setValue(pickerSamplesPixel);

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
        pickerSamplesLight_ = findViewById(R.id.pickerSamplesLight);
        if (pickerSamplesLight_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
        pickerSamplesLight_.setMinValue(1);
        pickerSamplesLight_.setMaxValue(maxSamplesLight);
        pickerSamplesLight_.setDisplayedValues(samplesLight);
        pickerSamplesLight_.setWrapSelectorWheel(true);
        pickerSamplesLight_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerSamplesLight_.setValue(pickerSamplesLight);

        pickerAccelerator_ = findViewById(R.id.pickerAccelerator);
        if (pickerAccelerator_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
        pickerAccelerator_.setMinValue(0);
        final String[] accelerators = {"Naive", "RegGrid", "BVH", "BVH2"};
        pickerAccelerator_.setMaxValue(accelerators.length - 1);
        pickerAccelerator_.setDisplayedValues(accelerators);
        pickerAccelerator_.setWrapSelectorWheel(true);
        pickerAccelerator_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerAccelerator_.setValue(pickerAccelerator);

        pickerThreads_ = findViewById(R.id.pickerThreads);
        if (pickerThreads_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
        pickerThreads_.setMinValue(1);
        final int maxCores = MainActivity.getNumberOfCores();
        pickerThreads_.setMaxValue(maxCores);
        pickerThreads_.setWrapSelectorWheel(true);
        pickerThreads_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerThreads_.setValue(pickerThreads);

        final int maxSizes = 9;
        final String[] sizes;
        try {
            sizes = new String[maxSizes];
        } catch (final OutOfMemoryError e) {
            e.fillInStackTrace();
            throw e;
        }
        sizes[0] = String.format(Locale.US, "%.2f", 0.05f) + 'x';
        for (int i = 2; i < maxSizes; i++) {
            final float value = (i + 1.0f) * 0.1f;
            sizes[i - 1] = String.format(Locale.US, "%.2f", value * value) + 'x';
        }
        sizes[maxSizes - 1] = String.format(Locale.US, "%.2f", 1.0f) + 'x';
        pickerSizes_ = findViewById(R.id.pickerSize);
        if (pickerSizes_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
        pickerSizes_.setMinValue(1);
        pickerSizes_.setMaxValue(maxSizes);
        pickerSizes_.setDisplayedValues(sizes);
        pickerSizes_.setWrapSelectorWheel(true);
        pickerSizes_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerSizes_.setValue(pickerSizes);
    }

    public void startRender(final View view) {
        switch (drawView_.viewText_.isWorking()) {
            case 0://if ray-tracer is idle
                if (pickerScene_.getDisplayedValues()[pickerScene_.getValue()].equals("OBJ")) {
                    final String obj = "WavefrontOBJs/CornellBox/CornellBox-Sphere";
                    //final String obj = "WavefrontOBJs/CornellBox/CornellBox-Water";
                    //final String obj = "WavefrontOBJs/CornellBox/CornellBox-Glossy";
                    //final String obj = "WavefrontOBJs/teapot/teapot";
                    //final String obj = "WavefrontOBJs/conference/conference";
                    //objText_ = readTextAsset(obj + ".obj");
                    //matText_ = readTextAsset(obj + ".mtl");
                    objText_ = obj + ".obj";
                    matText_ = obj + ".mtl";
                }
                final String str = pickerSizes_.getDisplayedValues()[pickerSizes_.getValue() - 1];
                drawView_.createScene(
                        pickerScene_.getValue(),
                        pickerShader_.getValue(),
                        pickerThreads_.getValue(),
                        pickerAccelerator_.getValue(),
                        Integer.parseInt(pickerSamplesPixel_.getDisplayedValues()
                                [pickerSamplesPixel_.getValue() - 1]
                        ),
                        Integer.parseInt(
                                pickerSamplesLight_.getDisplayedValues()
                                        [pickerSamplesLight_.getValue() - 1]
                        ),
                        Float.parseFloat(str.substring(0, str.indexOf('x'))),
                        objText_,
                        matText_,
                        getAssets()
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
