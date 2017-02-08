package puscas.mobilertapp;

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
import java.util.List;
import java.util.Locale;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import puscas.mobilertapp.DrawView.RenderTask.TouchTracker;

class DrawView extends LinearLayout {
    private long start_;
    private int numThreads_;
    private int frame_;
    private int period_;
    private int stage_;
    private int samples_;
    private boolean stopped_;
    private float timebase_;
    private float fps_;
    private Button buttonRender_;
    private TextView textView_;
    private Bitmap bitmap_;
    private LinearLayout layout_;
    private RenderTask renderTask_;
    private ScheduledExecutorService scheduler_;
    private BitmapDrawable bitmapDrawable_;

    DrawView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setWillNotDraw(false);
        this.setOnTouchListener(new TouchHandler());
    }

    private void FPS() {
        frame_++;
        float time = SystemClock.elapsedRealtime();
        if (time - timebase_ > 1000) {
            fps_ = frame_ * 1000.0f / (time - timebase_);
            timebase_ = time;
            frame_ = 0;
        }
    }

    private native void initialize(int scene, int shader, int width, int height, int sampler, int samples);

    private native void renderIntoBitmap(Bitmap image, int numThreads);

    private native void finish();

    private native void stopRender();

    private native void moveTouch(float x, float y, int primitiveId);

    private native int redraw(Bitmap bitmap);

    private native int traceTouch(float x, float y);

    private native float getFPS();

    private native long getTimeFrame();

    native int isWorking();

    void stopDrawing() {
        System.out.println("[stopDrawing," + Thread.currentThread().getId() + "]");
        stopRender();
        stopped_ = true;
    }

    void setButton(Button button) {
        buttonRender_ = button;
    }

    void startRender() {
        //System.out.println("[startRender," + Thread.currentThread().getId() + "]");
        period_ = 42;
        renderTask_ = new RenderTask();
        stopped_ = false;
        buttonRender_.setText(R.string.stop);
        start_ = SystemClock.elapsedRealtime();
        renderIntoBitmap(this.bitmap_, this.numThreads_);
        renderTask_.execute();
    }

    void createScene(int scene, int shader, int numThreads, TextView textView, int sampler, int samples) {
        //System.out.println("[createScene," + Thread.currentThread().getId() + "]");
        textView_ = textView;
        bitmap_ = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
        samples_ = samples;
        initialize(scene, shader, getWidth(), getHeight(), sampler, samples);
        numThreads_ = numThreads;
        frame_ = 0;
        timebase_ = 0.0f;
        fps_ = 0.0f;
        layout_ = this;
        bitmapDrawable_ = new BitmapDrawable(getResources(), bitmap_);
        setBackground(bitmapDrawable_);
    }

    private int getTouchListIndex(final int pointerID) {
        for (int i = 0; i < renderTask_.touches.size(); i++) {
            final TouchTracker thisTouch = renderTask_.touches.get(i);
            if (pointerID == thisTouch.pointerID_) {
                return i;
            }
        }
        return -1;
    }

    private enum Stage {
        IDLE(0), BUSY(1), ENDING(2), END(3), STOP(4);
        private final int id_;

        Stage(int id) {
            this.id_ = id;
        }

        public int getValue() {
            return this.id_;
        }
    }

    class RenderTask extends AsyncTask<Void, Void, Void> {
        final List<TouchTracker> touches = new ArrayList<>();
        final Runnable timer = new Runnable() {
            @Override
            public void run() {
                //System.out.println("[RenderTask," + Thread.currentThread().getId() + "]");
                for (int i = 0; i < touches.size(); i++) {
                    final TouchTracker touch = touches.get(i);
                    moveTouch(touch.x_, touch.y_, touch.primitiveID_);
                    //System.out.println("[run," + Thread.currentThread().getId() + "]" + "moveTouch (" + touch.x + "," + touch.y + ")");
                }
                stage_ = redraw(bitmap_);
                publishProgress();
                if (stopped_) {
                    scheduler_.shutdown();
                }
            }
        };

        @Override
        protected Void doInBackground(Void... params) {
            //System.out.println("[doInBackground," + Thread.currentThread().getId() + "]");
            scheduler_ = Executors.newSingleThreadScheduledExecutor();
            scheduler_.scheduleAtFixedRate(timer, 0, period_, TimeUnit.MILLISECONDS);
            boolean finished = false;
            do {
                try {
                    finished = scheduler_.awaitTermination(Integer.MAX_VALUE, TimeUnit.DAYS);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            } while (!finished);
            return null;
        }

        @Override
        protected void onProgressUpdate(Void... progress) {
            //System.out.println ("[onProgressUpdate," + Thread.currentThread().getId() + "]");
            layout_.setBackground(bitmapDrawable_);
            FPS();
            printText();
        }

        @Override
        protected void onPostExecute(Void result) {
            //System.out.println ("[onPostExecute," + Thread.currentThread().getId() + "]");
            redraw(bitmap_);
            FPS();
            printText();
            renderTask_.cancel(true);
            layout_.setBackground(bitmapDrawable_);
            finish();
            buttonRender_.setText(R.string.render);

            textView_ = null;
            layout_ = null;
            renderTask_ = null;
            scheduler_ = null;
            bitmapDrawable_ = null;
            bitmap_ = null;

            for (int i = 0; i < 4; i++) {
                System.gc();
                System.runFinalization();
            }
        }

        private void printText() {
            final String fpsRayTracer = String.format(Locale.US, "FPS:%.2f ", getFPS());
            final String fpsRender = String.format(Locale.US, "[%.2f], ", fps_);
            final String resolution = "R:" + getWidth() + "x" + getHeight() + ", ";
            final String threads = "T:" + numThreads_ + ", ";
            final String samples = "S:" + samples_ + ", ";
            final String timeFrame = String.format(Locale.US, "t:%.2fs ",
                    getTimeFrame() / 1000.0f);
            final String timeTotal = String.format(Locale.US, "[%.2fs]\n",
                    (SystemClock.elapsedRealtime() - start_) / 1000.0f);
            final String stage = Stage.values()[stage_] + ", ";
            final String allocated = "Malloc:" + Debug.getNativeHeapAllocatedSize() / 1024 + "KB ";
            final String available = "[Ma:" + Debug.getNativeHeapSize() / 1024 + "KB, ";
            final String free = "Mf:" + Debug.getNativeHeapFreeSize() / 1024 + "KB]";
            textView_.setText(fpsRayTracer + fpsRender +
                    resolution + threads + samples +
                    timeFrame + timeTotal +
                    stage + allocated + available + free);
        }

        class TouchTracker {
            final int pointerID_;
            final int primitiveID_;
            float x_;
            float y_;

            TouchTracker(final int pointerID, final int primitiveID, final float x, final float y) {
                super();
                this.pointerID_ = pointerID;
                this.primitiveID_ = primitiveID;
                this.x_ = x;
                this.y_ = y;
            }
        }
    }

    private class TouchHandler implements LinearLayout.OnTouchListener {
        @Override
        public boolean onTouch(View view, MotionEvent motionEvent) {
            if (stopped_ || stage_ != Stage.BUSY.getValue()) return false;
            switch (motionEvent.getActionMasked()) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN: {
                    final int actionIndex = motionEvent.getActionIndex();
                    final float x = motionEvent.getX(actionIndex);
                    final float y = motionEvent.getY(actionIndex);
                    //System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" + "ACTION_DOWN (" + x + "," + y + ")");
                    final int primitiveID = traceTouch(x, y);
                    if (primitiveID < 0) return false;
                    final int pointerID = motionEvent.getPointerId(actionIndex);
                    final TouchTracker thisTouch = renderTask_.new TouchTracker(pointerID, primitiveID, x, y);
                    renderTask_.touches.add(thisTouch);
                }
                return true;

                case MotionEvent.ACTION_MOVE:
                    for (int i = 0; i < motionEvent.getPointerCount(); i++) {
                        final int pointerID = motionEvent.getPointerId(i);
                        final int touchListIndex = getTouchListIndex(pointerID);
                        if (touchListIndex < 0) continue;
                        TouchTracker touch = renderTask_.touches.get(touchListIndex);
                        touch.x_ = motionEvent.getX(i);
                        touch.y_ = motionEvent.getY(i);
                        //System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" + "ACTION_MOVE (" + touch.x + "," + touch.y + ")");
                    }
                    return true;

                case MotionEvent.ACTION_POINTER_UP:
                    //System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" + "ACTION_POINTER_UP");
                    final int actionIndex = motionEvent.getActionIndex();
                    final int pointerID = motionEvent.getPointerId(actionIndex);
                    final int touchListIndex = getTouchListIndex(pointerID);
                    if (touchListIndex < 0) return false;
                    renderTask_.touches.remove(touchListIndex);
                    return true;

                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL:
                    //System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" + "ACTION_CANCEL");
                    renderTask_.touches.clear();
                    return true;

                default:
                    //System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" + "default");
                    return false;
            }
        }
    }
}