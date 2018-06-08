package puscas.mobilertapp;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.NumberPicker;
import android.widget.TextView;
import android.widget.Toast;

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
    private String objFile_;

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

    private void startStopRender() {
        final int scene = pickerScene_.getValue();
        final int shader = pickerShader_.getValue();
        final int threads = pickerThreads_.getValue();
        final int accelerator = pickerAccelerator_.getValue();
        final int samplesPixel = Integer.parseInt(pickerSamplesPixel_.getDisplayedValues()
                [pickerSamplesPixel_.getValue() - 1]);
        final int samplesLight = Integer.parseInt(pickerSamplesLight_.getDisplayedValues()
                [pickerSamplesLight_.getValue() - 1]);
        final String strResolution = pickerSizes_.getDisplayedValues()[pickerSizes_.getValue() - 1];
        final int width = Integer.parseInt(strResolution.substring(0, strResolution.indexOf('x')));
        final int height = Integer.parseInt(strResolution.substring(strResolution.indexOf('x') + 1, strResolution.length()));
        final String strScene = pickerScene_.getDisplayedValues()[pickerScene_.getValue()];
        final String objText = objFile_ + ".obj";
        final String matText = objFile_ + ".mtl";

        switch (ViewText.isWorking()) {
            case 0:
            case 2:
            case 3://if ray-tracer is idle
                drawView_.createScene(scene, shader, threads, accelerator, samplesPixel, samplesLight, width, height, objText, matText);
                drawView_.startRender();
                break;

            default://if ray-tracer is busy
                this.drawView_.stopDrawing();
                break;
        }
    }

    public void onDetachedFromWindow() {
        super.onDetachedFromWindow();
    }

    public void onDestroy() {
        super.onDestroy();
        drawView_.onDestroy();
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == Activity.RESULT_OK) {
            final Uri uri = data.getData();
            final String sdCardDir = Environment.getExternalStorageDirectory() + "/";
            if (uri != null) {
                String filePath = uri.getEncodedPath();
                filePath = filePath.replace("%2F", "/");
                filePath = filePath.replace("%3A", "/");
                filePath = filePath.replace("/document/primary/", sdCardDir);

                final int lastIndex = filePath.lastIndexOf('.');
                objFile_ = filePath.substring(0, lastIndex);
                startStopRender();
            }
        }
    }

    private void showFileChooser() {
        final int FILE_SELECT_CODE = 1;
        final Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("*/*");
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        try {
            final Intent intentChooseFile = Intent.createChooser(intent, "Select a File to Upload");
            startActivityForResult(intentChooseFile, FILE_SELECT_CODE);
            //objFile_ = Environment.getExternalStorageDirectory() + "/WavefrontOBJs/conference/conference";
            //startStopRender();
        } catch (android.content.ActivityNotFoundException ex) {
            Toast.makeText(this, "Please install a File Manager.", Toast.LENGTH_SHORT).show();
        }
    }

    @TargetApi(Build.VERSION_CODES.M)
    public boolean CheckStoragePermission() {
        final int PERMISSION_STORAGE = 1;
        final int permissionCheckRead = ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE);
        if (permissionCheckRead != PackageManager.PERMISSION_GRANTED) {
            final String[] permissions = new String[]{Manifest.permission.READ_EXTERNAL_STORAGE};
            ActivityCompat.requestPermissions(this, permissions, PERMISSION_STORAGE);
            return false;
        } else
            return true;
    }

    @Override
    public void onRestoreInstanceState(final Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        final int scene = savedInstanceState.getInt("pickerScene");
        final int shader = savedInstanceState.getInt("pickerShader");
        final int threads = savedInstanceState.getInt("pickerThreads");
        final int accelerator = savedInstanceState.getInt("pickerAccelerator");
        final int samplesPixel = savedInstanceState.getInt("pickerSamplesPixel");
        final int samplesLight = savedInstanceState.getInt("pickerSamplesLight");
        final int sizes = savedInstanceState.getInt("pickerSizes");
        pickerScene_.setValue(scene);
        pickerShader_.setValue(shader);
        pickerThreads_.setValue(threads);
        pickerAccelerator_.setValue(accelerator);
        pickerSamplesPixel_.setValue(samplesPixel);
        pickerSamplesLight_.setValue(samplesLight);
        pickerSizes_.setValue(sizes);
    }

    @Override
    public void onSaveInstanceState(final Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        final int scene = pickerScene_.getValue();
        final int shader = pickerShader_.getValue();
        final int threads = pickerThreads_.getValue();
        final int accelerator = pickerAccelerator_.getValue();
        final int samplesPixel = pickerSamplesPixel_.getValue();
        final int samplesLight = pickerSamplesLight_.getValue();
        final int sizes = pickerSizes_.getValue();
        savedInstanceState.putInt("pickerScene", scene);
        savedInstanceState.putInt("pickerShader", shader);
        savedInstanceState.putInt("pickerThreads", threads);
        savedInstanceState.putInt("pickerAccelerator", accelerator);
        savedInstanceState.putInt("pickerSamplesPixel", samplesPixel);
        savedInstanceState.putInt("pickerSamplesLight", samplesLight);
        savedInstanceState.putInt("pickerSizes", sizes);
        DrawView.finishRender();
        drawView_.freeArrays();
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
        final boolean res = num_config[0] > 0;
        return res;
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
            //drawView_ = new DrawView(this);
            //setContentView(drawView_);
            //setContentView(R.layout.activity_main);
            if (drawView_ == null) {
                Log.e("DrawView", "DrawView is NULL !!!");
                System.exit(0);
            }
            drawView_.setVisibility(View.INVISIBLE);
            drawView_.setEGLContextClientVersion(2);
            drawView_.setEGLConfigChooser(8, 8, 8, 8, 24, 0);
            final MainRenderer renderer = new MainRenderer();
            drawView_.renderer_ = renderer;
            drawView_.setRenderer(renderer);
            drawView_.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
            drawView_.renderer_.bitmap_ = Bitmap.createBitmap(1, 1, Bitmap.Config.ARGB_8888);
            drawView_.renderer_.bitmap_.eraseColor(Color.DKGRAY);
            final String vertexShader = readTextAsset("Shaders/VertexShader.glsl");
            final String fragmentShader = readTextAsset("Shaders/FragmentShader.glsl");
            final String vertexShaderRaster = readTextAsset("Shaders/VertexShaderRaster.glsl");
            final String fragmentShaderRaster = readTextAsset("Shaders/FragmentShaderRaster.glsl");
            drawView_.renderer_.vertexShaderCode = vertexShader;
            drawView_.renderer_.fragmentShaderCode = fragmentShader;
            drawView_.renderer_.vertexShaderCodeRaster = vertexShaderRaster;
            drawView_.renderer_.fragmentShaderCodeRaster = fragmentShaderRaster;
            drawView_.setVisibility(View.VISIBLE);

            /*if (supportES2 && checkGL20Support()) {
                //View view = GLSurfaceView.inflate(this, R.layout.activity_main, null);
                //drawView_ = new DrawView(this);
                drawView_.setVisibility(View.GONE);
                drawView_.setEGLContextClientVersion(2);
                DrawView myLayout = findViewById(R.id.drawLayout);
                final MainRenderer renderer2 = new MainRenderer();
                drawView_.clearFocus();
                drawView_.clearAnimation();
                drawView_.setVisibility(View.INVISIBLE);
                drawView_.destroyDrawingCache();
                drawView_.renderer_ = renderer;
                drawView_.setRenderer(renderer);
                drawView_.setVisibility(View.VISIBLE);
            }*/

            drawView_.viewText_.buttonRender_ = findViewById(R.id.renderButton);
            if (drawView_.viewText_.buttonRender_ == null) {
                Log.e("Button", "Button is NULL !!!");
                System.exit(0);
            }
            drawView_.viewText_.buttonRender_.setOnLongClickListener((View v) -> {
                this.recreate();
                return false;
            });
            final TextView textView = findViewById(R.id.timeText);
            if (textView == null) {
                Log.e("ViewText", "ViewText is NULL !!!");
                System.exit(0);
            }
            drawView_.setView(textView);
            drawView_.setPreserveEGLContextOnPause(true);
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
        final String[] accelerators = {"Naive", "RegGrid", "BVH", "None"};
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
        final int scene = pickerScene_.getValue();
        final int isWorking = ViewText.isWorking();
        if (scene >= 4 && isWorking != 1) {
            final boolean permissionToReadFile = CheckStoragePermission();
            if (permissionToReadFile) {
                showFileChooser();
            }
            return;
        }
        startStopRender();
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
