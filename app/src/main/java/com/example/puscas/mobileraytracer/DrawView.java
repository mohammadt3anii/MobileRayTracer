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
    private RenderTask renderTask_;
    private ScheduledExecutorService scheduler_;

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
        renderTask_ = new RenderTask(bitmap_, period);
        buttonRender_.setText(R.string.stop);
        this.start_ = SystemClock.elapsedRealtime();
        drawIntoBitmap(this.bitmap_, this.numThreads_);
        renderTask_.execute();
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

    private enum Stage {
        IDLE(0), BUSY(1), END(2), STOP(3);
        private final int id;

        Stage(int id) {
            this.id = id;
        }

        public int getValue() {
            return id;
        }
    }

    private class RenderTask extends AsyncTask<Void, Void, Void> {
        private Bitmap bitmap_;
        private int stage_;
        //private int count1 = 0;
        final Runnable timer = new Runnable() {
            public void run() {
                stage_ = redraw(bitmap_);
                publishProgress();
                //System.out.println (count1++ + " -> " + Thread.currentThread().getId() + " mandou atualizar ecra");
                if (stage_ == 2 || stage_ == 3) {
                    scheduler_.shutdown();
                }
            }
        };
        private int period_;

        RenderTask(Bitmap b, int period) {
            bitmap_ = b;
            period_ = period;
        }

        protected Void doInBackground(Void... params) {
            scheduler_ = Executors.newSingleThreadScheduledExecutor();
            scheduler_.scheduleAtFixedRate(timer, 0, period_, TimeUnit.MILLISECONDS);
            boolean finished = false;
            do {
                Thread.yield();
                try {
                    //System.out.println (Thread.currentThread().getId() + " vai esperar");
                    finished = scheduler_.awaitTermination(Integer.MAX_VALUE, TimeUnit.DAYS);
                    //System.out.println (Thread.currentThread().getId() + " acordou");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            } while (!finished);
            scheduler_.shutdownNow();
            return null;
        }

        //private int count2 = 0;
        protected void onProgressUpdate(Void... progress) {
            mLinearLayout.setBackground(new BitmapDrawable(getResources(), bitmap_));
            FPS();
            printText();
        }

        protected void onPostExecute(Void result) {
            finish();
            printText();
            renderTask_.cancel(true);
            buttonRender_.setText(R.string.render);
            System.gc();
            System.runFinalization();
            //System.out.println (Thread.currentThread().getId() + " diz que terminou tudo");
        }

        private void printText() {
            final double allocated = Debug.getNativeHeapAllocatedSize() / 1048576;
            final double available = Debug.getNativeHeapSize() / 1048576;
            final double free = Debug.getNativeHeapFreeSize() / 1048576;
            final long millisec = SystemClock.elapsedRealtime();
            final float sec = (millisec - start_) / 1000.0f;
            final String time = String.format(java.util.Locale.US, "%.2f", sec);
            textView_.setText("FPS: " + String.format(java.util.Locale.US, "%.2f", FPS) + ", " + Stage.values()[stage_] + ", " + text_ + time + "s \nMemory -> alloc:"
                    + allocated + "MB, [a:" + available + "MB, f:" + free + "MB]");
            //System.out.println (count2++ + " -> " + Thread.currentThread().getId() + " atualizou ecra");
        }
    }
}