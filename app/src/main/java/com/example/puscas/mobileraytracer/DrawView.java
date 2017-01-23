package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.AsyncTask;
import android.os.Debug;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

class DrawView extends LinearLayout
{
    private long start_;
    private long finished_;
    private int numThreads_;
    private int frame_;
    private int period_;
    private int stage_;
    private boolean stopped_;
    private float timebase_;
    private float fps_;
    private Button buttonRender_;
    private String text_;
    private TextView textView_;
    private Bitmap bitmap_;
    private LinearLayout layout_;
    private RenderTask renderTask_;
    private ScheduledExecutorService scheduler_;
    private BitmapDrawable bitmapDrawable_;

    public DrawView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        setWillNotDraw(false);
        this.setOnTouchListener(new TouchHandler());
    }

    void FPS() {
        frame_++;
        final float time = SystemClock.elapsedRealtime();
        if (time - timebase_ > 1000) {
            fps_ = frame_ * 1000.0f / (time - timebase_);
            timebase_ = time;
            frame_ = 0;
        }
    }

    private native void initialize(int scene, int shader, int width, int height, int sampler, int samples);
    private native void drawIntoBitmap(Bitmap image, int numThreads);
    private native void finish();
    private native void clearStage();
    private native int redraw(Bitmap bitmap);
    private native void stopRender();
    private native int traceTouch(float x, float y);
    private native void moveTouch(float x, float y, int primitiveId);

    private native float getFPS();
    native int isWorking();

    void stopDrawing() {
        System.out.println("[stopDrawing," + Thread.currentThread().getId() + "]");
        stopRender();
        stopped_ = true;
    }

    void setButton(Button button) {
        buttonRender_ = button;
    }

    void startRender(int period) {
        //System.out.println("[startRender," + Thread.currentThread().getId() + "]");
        period_ = period;
        renderTask_ = new RenderTask();
        finished_ = 0;
        stopped_ = false;
        buttonRender_.setText(R.string.stop);
        start_ = SystemClock.elapsedRealtime();
        drawIntoBitmap(this.bitmap_, this.numThreads_);
        renderTask_.execute();
    }

    void createScene(int scene, int shader, int numThreads, TextView textView, int sampler, int samples)
    {
        //System.out.println("[createScene," + Thread.currentThread().getId() + "]");
        textView_ = textView;
        bitmap_ = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
        initialize(scene, shader, getWidth(), getHeight(), sampler, samples);
        numThreads_ = numThreads;
        text_ = "HAl:" + this.isHardwareAccelerated() + ", R:" + getWidth() + "x" + getHeight() + ", T:" + this.numThreads_ + ", S:" + samples + ", t:";
        frame_ = 0;
        timebase_ = 0.0f;
        fps_ = 0.0f;
        layout_ = this;
        bitmapDrawable_ = new BitmapDrawable(getResources(), bitmap_);
        setBackground(bitmapDrawable_);
    }

    private int getTouchListIndex(int pointerID) {
        for (int i = 0; i < renderTask_.touches.size(); i++) {
            RenderTask.TouchTracker thisTouch = renderTask_.touches.get(i);
            if (pointerID == thisTouch.pointerID)
                return i;
        }
        return -1;
    }

    private enum Stage {
        IDLE(0), BUSY(1), ENDING(2), END(3), STOP(4);
        private final int id;

        Stage(int id) {
            this.id = id;
        }

        public int getValue() {
            return id;
        }
    }

    private class RenderTask extends AsyncTask<Void, Void, Void> {
        ArrayList<TouchTracker> touches = new ArrayList<>();
        final Runnable timer = new Runnable() {
            public void run() {
                //System.out.println("[RenderTask," + Thread.currentThread().getId() + "]");
                for (int i = 0; i < touches.size(); i++) {
                    TouchTracker touch = touches.get(i);
                    moveTouch(touch.x, touch.y, touch.primitiveID);
                }
                stage_ = redraw(bitmap_);
                publishProgress();
                if (stage_ == Stage.ENDING.getValue()) {
                    finished_ = SystemClock.elapsedRealtime();
                    clearStage();
                }
                if (stopped_) {
                    scheduler_.shutdown();
                }
            }
        };

        protected Void doInBackground(Void... params) {
            //System.out.println("[doInBackground," + Thread.currentThread().getId() + "]");
            scheduler_ = Executors.newSingleThreadScheduledExecutor();
            scheduler_.scheduleAtFixedRate(timer, 0, period_, TimeUnit.MILLISECONDS);
            boolean finished = false;
            do {
                Thread.yield();
                try {
                    finished = scheduler_.awaitTermination(Integer.MAX_VALUE, TimeUnit.DAYS);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            } while (!finished);
            return null;
        }

        protected void onProgressUpdate(Void... progress) {
            //System.out.println ("[onProgressUpdate," + Thread.currentThread().getId() + "]");
            layout_.setBackground(bitmapDrawable_);
            FPS();
            printText();
        }

        protected void onPostExecute(Void result) {
            //System.out.println ("[onPostExecute," + Thread.currentThread().getId() + "]");
            redraw(bitmap_);
            FPS();
            printText();
            renderTask_.cancel(true);
            layout_.setBackground(bitmapDrawable_);
            finish();
            buttonRender_.setText(R.string.render);

            text_ = null;
            textView_ = null;
            layout_ = null;
            renderTask_ = null;
            scheduler_ = null;
            bitmapDrawable_ = null;
            bitmap_ = null;

            int i = 1;
            do {
                System.gc();
                System.runFinalization();
            } while (i++ < 4);
        }

        private void printText() {
            final long millisec = finished_ > 0 ? finished_ : SystemClock.elapsedRealtime();
            final float sec = (millisec - start_) / 1000.0f;
            final String time = String.format(java.util.Locale.US, "%.2f", sec);
            final double allocated = Debug.getNativeHeapAllocatedSize() / 1024;
            final double available = Debug.getNativeHeapSize() / 1024;
            final double free = Debug.getNativeHeapFreeSize() / 1024;
            textView_.setText(
                    "FPS:" + String.format(java.util.Locale.US, "%.2f", getFPS()) + " [" + String.format(java.util.Locale.US, "%.2f", fps_) + "], " +
                            Stage.values()[stage_] + ", " + text_ + time + "s \nMemory -> alloc:" +
                            allocated + "KB, [a:" + available + "KB, f:" + free + "KB]");
        }

        class TouchTracker {
            int pointerID;
            int primitiveID;
            float x;
            float y;
        }
    }

    class TouchHandler implements LinearLayout.OnTouchListener {
        public boolean onTouch(View view, MotionEvent motionEvent) {
            if (stopped_ || stage_ != Stage.BUSY.getValue()) return false;
            final int actionIndex = motionEvent.getActionIndex();
            final float x = motionEvent.getX(actionIndex);
            final float y = motionEvent.getY(actionIndex);
            System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" + motionEvent.getActionMasked());
            System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" + x + "," + y);
            switch (motionEvent.getActionMasked()) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN: {
                    final int primitiveID = traceTouch(x, y);
                    final int pointerID = motionEvent.getPointerId(actionIndex);
                    RenderTask.TouchTracker thisTouch = renderTask_.new TouchTracker();
                    thisTouch.pointerID = pointerID;
                    thisTouch.primitiveID = primitiveID;
                    thisTouch.x = x;
                    thisTouch.y = y;
                    renderTask_.touches.add(thisTouch);
                }
                break;

                case MotionEvent.ACTION_POINTER_UP: {
                    final int pointerID = motionEvent.getPointerId(actionIndex);
                    // Find the matching touch and remove it.
                    final int touchListIndex = getTouchListIndex(pointerID);
                    if (touchListIndex == -1)
                        return false;
                    renderTask_.touches.remove(touchListIndex);
                }
                break;

                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL:
                    renderTask_.touches.clear();
                    break;

                case MotionEvent.ACTION_MOVE: {
                    for (int i = 0; i < motionEvent.getPointerCount(); i++) {
                        // Find the matching touch.
                        final int pointerID = motionEvent.getPointerId(i);
                        final int touchListIndex = getTouchListIndex(pointerID);
                        if (touchListIndex == -1)
                            continue;
                        RenderTask.TouchTracker touch = renderTask_.touches.get(touchListIndex);
                        // Update its position.
                        touch.x = motionEvent.getX(i);
                        touch.y = motionEvent.getY(i);
                    }
                }
                break;

                default:
                    break;
            }
            return true;
        }
    }
}