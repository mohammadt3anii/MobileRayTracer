package puscas.mobilertapp;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.NumberPicker;
import android.widget.TextView;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.io.InputStream;
import java.util.Locale;
import java.util.regex.Pattern;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

public final class MainActivity extends Activity {

    static {
        try {
            System.loadLibrary("MobileRT");
            System.loadLibrary("Components");
            System.loadLibrary("AppInterface");
        } catch (Exception e) {
            Log.e("LINK ERROR", "WARNING: Could not load native library: " + e.getMessage());
            System.exit(1);
        }
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
        int res = 0;
        try {
            final File dir = new File("/sys/devices/system/cpu/");
            final File[] files = dir.listFiles(new CpuFilter());
            res = files.length;
        } catch (final RuntimeException ignored) {
            Log.e("getNumCoresOldPhones", "Can't get number of cores available!!!");
            System.exit(1);
        }
        return res;
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
            System.exit(1);
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

    private boolean checkGL20Support() {
        final EGL10 egl = (EGL10) EGLContext.getEGL();
        final EGLDisplay display = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);

        final int[] version = new int[2];
        egl.eglInitialize(display, version);

        final int EGL_OPENGL_ES2_BIT = 4;
        final int[] configAttribs = {
                EGL10.EGL_RED_SIZE, 4,
                EGL10.EGL_GREEN_SIZE, 4,
                EGL10.EGL_BLUE_SIZE, 4,
                EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL10.EGL_NONE
        };

        final EGLConfig[] configs = new EGLConfig[10];
        final int[] num_config = new int[1];
        egl.eglChooseConfig(display, configAttribs, configs, 10, num_config);
        egl.eglTerminate(display);
        return num_config[0] > 0;
    }

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        int defaultPickerScene = 0;
        int defaultPickerShader = 0;
        int defaultPickerThreads = 1;
        int defaultPickerAccelerator = 0;
        int defaultPickerSamplesPixel = 1;
        int defaultPickerSamplesLight = 1;
        int defaultPickerSizes = 4;
        if (savedInstanceState != null) {
            defaultPickerScene = savedInstanceState.getInt("pickerScene");
            defaultPickerShader = savedInstanceState.getInt("pickerShader");
            defaultPickerThreads = savedInstanceState.getInt("pickerThreads");
            defaultPickerAccelerator = savedInstanceState.getInt("pickerAccelerator");
            defaultPickerSamplesPixel = savedInstanceState.getInt("pickerSamplesPixel");
            defaultPickerSamplesLight = savedInstanceState.getInt("pickerSamplesLight");
            defaultPickerSizes = savedInstanceState.getInt("pickerSizes");
        }

        try {
            setContentView(R.layout.activity_main);
        } catch (final RuntimeException e) {
            Log.e("RuntimeException", e.getMessage());
            System.exit(1);
        }

        final ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        assert am != null;
        final ConfigurationInfo info = am.getDeviceConfigurationInfo();
        final boolean supportES2 = (info.reqGlEsVersion >= 0x20000);
        if (supportES2 && checkGL20Support()) {
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
            drawView_.renderer_.bitmap_ = Bitmap.createBitmap(1, 1, Bitmap.Config.ARGB_8888);
            drawView_.renderer_.bitmap_.eraseColor(Color.DKGRAY);
            drawView_.setVisibility(View.VISIBLE);
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
        pickerScene_.setWrapSelectorWheel(true);
        pickerScene_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerScene_.setValue(defaultPickerScene);
        pickerScene_.setDisplayedValues(scenes);

        pickerShader_ = findViewById(R.id.pickerShader);
        if (pickerShader_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }

        pickerShader_.setMinValue(0);
        final String[] shaders = {"NoShadows", "Whitted", "PathTracer", "DepthMap", "Diffuse"};
        pickerShader_.setMaxValue(shaders.length - 1);
        pickerShader_.setWrapSelectorWheel(true);
        pickerShader_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerShader_.setValue(defaultPickerShader);
        pickerShader_.setDisplayedValues(shaders);

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
        pickerSamplesPixel_.setWrapSelectorWheel(true);
        pickerSamplesPixel_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerSamplesPixel_.setValue(defaultPickerSamplesPixel);
        pickerSamplesPixel_.setDisplayedValues(samplesPixel);

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
        pickerSamplesLight_.setWrapSelectorWheel(true);
        pickerSamplesLight_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerSamplesLight_.setValue(defaultPickerSamplesLight);
        pickerSamplesLight_.setDisplayedValues(samplesLight);

        pickerAccelerator_ = findViewById(R.id.pickerAccelerator);
        if (pickerAccelerator_ == null) {
            Log.e("NumberPicker", "NumberPicker is NULL !!!");
            System.exit(0);
        }
        pickerAccelerator_.setMinValue(0);
        final String[] accelerators = {"Naive", "RegGrid", "BVH", "BVH_vec"};
        pickerAccelerator_.setMaxValue(accelerators.length - 1);
        pickerAccelerator_.setWrapSelectorWheel(true);
        pickerAccelerator_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerAccelerator_.setValue(defaultPickerAccelerator);
        pickerAccelerator_.setDisplayedValues(accelerators);

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
        pickerThreads_.setValue(defaultPickerThreads);

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
        pickerSizes_.setWrapSelectorWheel(true);
        pickerSizes_.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        pickerSizes_.setValue(defaultPickerSizes);

        ViewTreeObserver vto = drawView_.getViewTreeObserver();
        vto.addOnGlobalLayoutListener(() -> {
            final int width = drawView_.getWidth();
            final int height = drawView_.getHeight();

            float size = 0.05f;
            int currentWidth = DrawView.resize(Math.round(width * size));
            int currentHeight = DrawView.resize(Math.round(height * size));
            sizes[0] = "" + currentWidth + 'x' + currentHeight;
            for (int i = 2; i < maxSizes; i++) {
                size = (i + 1.0f) * 0.1f;
                currentWidth = DrawView.resize(Math.round(width * size * size));
                currentHeight = DrawView.resize(Math.round(height * size * size));
                sizes[i - 1] = "" + currentWidth + 'x' + currentHeight;
            }
            size = 1.0f;
            currentWidth = DrawView.resize(Math.round(width * size * size));
            currentHeight = DrawView.resize(Math.round(height * size * size));
            sizes[maxSizes - 1] = "" + currentWidth + 'x' + currentHeight;

            pickerSizes_.setDisplayedValues(sizes);
        });
    }

    public void startRender(final View view) {
        switch (ViewText.isWorking()) {
            case 0:
            case 2:
            case 3://if ray-tracer is idle
                if (pickerScene_.getDisplayedValues()[pickerScene_.getValue()].equals("OBJ")) {
                    //final String obj = "WavefrontOBJs/CornellBox/CornellBox-Sphere";
                    //final String obj = "WavefrontOBJs/CornellBox/CornellBox-Water";
                    //final String obj = "WavefrontOBJs/CornellBox/CornellBox-Glossy";
                    //final String obj = "WavefrontOBJs/teapot/teapot";
                    final String obj = "WavefrontOBJs/conference/conference";
                    //objText_ = readTextAsset(obj + ".obj");
                    //matText_ = readTextAsset(obj + ".mtl");
                    objText_ = obj + ".obj";
                    matText_ = obj + ".mtl";
                }
                final String str = pickerSizes_.getDisplayedValues()[pickerSizes_.getValue() - 1];
                final int width = Integer.parseInt(str.substring(0, str.indexOf('x')));
                final int height = Integer.parseInt(str.substring(str.indexOf('x') + 1, str.length()));
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
                        width,
                        height,
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
