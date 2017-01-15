package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Debug;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.view.SurfaceView;
import android.widget.Button;
import android.widget.TextView;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

import static android.graphics.Bitmap.createBitmap;
import static java.util.concurrent.TimeUnit.MILLISECONDS;
import static java.util.concurrent.TimeUnit.SECONDS;

public class DrawView extends SurfaceView
{
    private long start_;
    private TextView textView_;
    private Bitmap bitmapW_;
    private Button buttonRender_;
    private int numThreads_;
    private String text_;
    private ScheduledExecutorService scheduler_;
    private int frame = 0;
    private float timebase = 0.0f;
    private float FPS = 0.0f;

    public DrawView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        setWillNotDraw(false);
    }

    String FPS() {
        frame++;
        float time = SystemClock.elapsedRealtime();
        if (time - timebase > 1000) {
            final float fps = frame * 1000.0f / (time - timebase);
            System.out.println();
            timebase = time;
            frame = 0;
            FPS = fps;
        }
        return "FPS: " + FPS;
    }

    private native void initialize(int scene, int shader, int width, int height, int sampler, int samples);
    private native void drawIntoBitmap(Bitmap image, int numThreads);
    private native void finish();
    native void stopRender();
    native int isWorking();

    void stopTimer() {
        scheduler_.shutdown();
        scheduler_.shutdownNow();
        try {
            while (!scheduler_.awaitTermination(10, SECONDS)) {
                System.out.println("À ESPERA ...");
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        finish();
        this.buttonRender_.setText(R.string.render);
    }

    void setButton(Button button) {
        buttonRender_ = button;
    }

    void startRender(int period) {
        this.start_ = SystemClock.elapsedRealtime();
        drawIntoBitmap(this.bitmapW_, this.numThreads_);
        final Runnable timer = new Runnable() {
            public void run() {
                postInvalidate();
            }
        };
        scheduler_ = Executors.newScheduledThreadPool(1);
        scheduler_.scheduleWithFixedDelay(timer, 0, period, MILLISECONDS);
        this.buttonRender_.setText(R.string.stop);
    }

    void createScene(int scene, int shader, int numThreads, TextView textView, int sampler, int samples)
    {
        int width = getWidth();
        int height = getHeight();
        textView_ = textView;
        bitmapW_ = createBitmap(width, height, Bitmap.Config.ARGB_8888).copy(Bitmap.Config.ARGB_8888, false);
        initialize(scene, shader, width, height, sampler, samples);
        numThreads_ = numThreads;

        text_ = "HAv:" + this.isHardwareAccelerated() + ", R:" + width + "x" + height + ", T:" + this.numThreads_ + ", S:" + samples + ", t:";
        //mSurfaceView = (SurfaceView) this.findViewById(R.id.Surface);
        //mSurfaceHolder = mSurfaceView.getHolder();
    }

    public void onDraw(Canvas canvas)
    {
        this.setWillNotDraw(false);
        if (!isInEditMode())
        {
            String stage;
            switch (isWorking()) {
                case 0:
                {
                    canvas.drawBitmap(bitmapW_, 0.0f, 0.0f, null);
                }
                return;

                case 1://While ray-tracer is busy
                {
                    canvas.drawBitmap(bitmapW_.copy(Bitmap.Config.ARGB_8888, false), 0.0f, 0.0f, null);
                    stage = "Running -> ";
                }
                    break;

                case 2://When ray-tracer is finished
                {
                    canvas.drawBitmap(bitmapW_, 0.0f, 0.0f, null);
                    stopTimer();
                    stage = "Finished -> ";
                }
                break;

                case 3://When ray-tracer is stopped
                {
                    canvas.drawBitmap(bitmapW_, 0.0f, 0.0f, null);
                    stopTimer();
                    stage = "Stopped -> ";
                }
                    break;

                default:
                    stage = "Unknown -> ";
                    break;
            }
            double allocated = Debug.getNativeHeapAllocatedSize() / 1048576;
            double available = Debug.getNativeHeapSize() / 1048576;
            double free = Debug.getNativeHeapFreeSize() / 1048576;
            textView_.setText(FPS() + ", " + stage + this.text_ + (SystemClock.elapsedRealtime() - this.start_) + "ms \nMemory -> alloc:"
                    + allocated + "MB, [available:" + available + "MB, free:" + free + "MB], " +
                    "HAc:" + canvas.isHardwareAccelerated());
        }
    }
}