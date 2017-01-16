package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.AsyncTask;
import android.os.Debug;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

class DrawView extends LinearLayout
{
    private long start_;
    private TextView textView_;
    private Bitmap bitmap_;
    private Button buttonRender_;
    private int numThreads_;
    private String text_;
    private int frame = 0;
    private float timebase = 0.0f;
    private float FPS = 0.0f;
    private LinearLayout mLinearLayout = this;

    public DrawView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        setWillNotDraw(false);
    }

    void FPS() {
        frame++;
        final float time = SystemClock.elapsedRealtime();
        if (time - timebase > 1000) {
            FPS = frame * 1000.0f / (time - timebase);
            timebase = time;
            frame = 0;
        }
    }

    private native void initialize(int scene, int shader, int width, int height, int sampler, int samples);
    private native void drawIntoBitmap(Bitmap image, int numThreads);
    private native void finish();

    private native int redraw(Bitmap bitmap);
    native void stopRender();
    native int isWorking();

    void setButton(Button button) {
        buttonRender_ = button;
    }

    void startRender(int period) {
        RaytraceTask raytraceThread = new RaytraceTask(bitmap_, period);
        raytraceThread.execute();
        buttonRender_.setText(R.string.stop);
        this.start_ = SystemClock.elapsedRealtime();
        drawIntoBitmap(this.bitmap_, this.numThreads_);
    }

    void createScene(int scene, int shader, int numThreads, TextView textView, int sampler, int samples)
    {
        int width_ = getWidth();
        int height_ = getHeight();
        textView_ = textView;
        bitmap_ = Bitmap.createBitmap(width_, height_, Bitmap.Config.ARGB_8888).copy(Bitmap.Config.ARGB_8888, false);
        setBackground(new BitmapDrawable(mLinearLayout.getResources(), bitmap_));
        bitmap_.setHasAlpha(false);
        initialize(scene, shader, width_, height_, sampler, samples);
        numThreads_ = numThreads;

        text_ = "HAl:" + this.isHardwareAccelerated() + ", R:" + width_ + "x" + height_ + ", T:" + this.numThreads_ + ", S:" + samples + ", t:";
    }

    private class RaytraceTask extends AsyncTask<Void, Void, Void> {
        private Bitmap bitmap_;
        private int stage_;
        private int period_;

        RaytraceTask(Bitmap b, int period) {
            bitmap_ = b;
            period_ = period;
        }

        protected Void doInBackground(Void... params) {
            final Runnable timer = new Runnable() {
                public void run() {
                    stage_ = redraw(bitmap_);
                    publishProgress();
                }
            };
            ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);
            scheduler.scheduleAtFixedRate(timer, 0, period_, TimeUnit.MILLISECONDS);
            do {
                Thread.yield();
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            } while (stage_ != 2 && stage_ != 3);
            scheduler.shutdown();
            return null;
        }

        protected void onProgressUpdate(Void... progress) {
            mLinearLayout.setBackground(new BitmapDrawable(mLinearLayout.getResources(), bitmap_));
            final double allocated = Debug.getNativeHeapAllocatedSize() / 1048576;
            final double available = Debug.getNativeHeapSize() / 1048576;
            final double free = Debug.getNativeHeapFreeSize() / 1048576;
            final long millisec = SystemClock.elapsedRealtime();
            final float sec = (millisec - start_) / 1000.0f;
            final String time = String.format(java.util.Locale.US, "%.2f", sec);
            FPS();
            textView_.setText("FPS: " + String.format(java.util.Locale.US, "%.2f", FPS) + ", " + stage_ + ", " + text_ + time + "s \nMemory -> alloc:"
                    + allocated + "MB, [a:" + available + "MB, f:" + free + "MB]");
        }

        protected void onPostExecute(Void result) {
            finish();
            buttonRender_.setText(R.string.render);
        }
    }
}

/*
    public void onDraw(Canvas canvas)
    {
        this.setWillNotDraw(false);
        if (!isInEditMode())
        {
            String stage;
            switch (isWorking()) {
                case 0:
                {
                    canvas.drawBitmap(bitmap_, 0.0f, 0.0f, null);
                }
                return;

                case 1://While ray-tracer is busy
                {
                    redraw(bitmap_);
                    canvas.drawBitmap(bitmap_, 0.0f, 0.0f, null);
                    stage = "Running -> ";
                }
                    break;

                case 2://When ray-tracer is finished
                {
                    redraw(bitmap_);
                    canvas.drawBitmap(bitmap_, 0.0f, 0.0f, null);
                    stopTimer();
                    stage = "Finished -> ";
                }
                break;

                case 3://When ray-tracer is stopped
                {
                    redraw(bitmap_);
                    canvas.drawBitmap(bitmap_, 0.0f, 0.0f, null);
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
*/