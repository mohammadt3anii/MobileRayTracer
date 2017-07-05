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

final class DrawView extends LinearLayout {
    long start_ = 0L;
    long period_ = 0L;
    int stage_ = 0;
    float fps_ = 0.0f;
    Button buttonRender_ = null;
    Bitmap bitmap_ = null;
    RenderTask renderTask_ = null;
    ScheduledExecutorService scheduler_ = null;
    String stageT_ = null;
    String fpsT_ = null;
    String timeFrameT_ = null;
    String timeT_ = null;
    String fpsRenderT_ = null;
    String allocatedT_ = null;
    String sampleT_ = null;
    private int numThreads_ = 0;
    private int frame_ = 0;
    private float timebase_ = 0.0f;
    private TextView textView_ = null;
    private String resolutionT_ = null;
    private String threadsT_ = null;
    private String samplesPixelT_ = null;
    private String samplesLightT_ = null;
    //private String availableT_ = null;
    //private String freeT_ = null;

    public DrawView(final Context context, final AttributeSet attrs) {
        super(context, attrs);
        setWillNotDraw(false);
        resetPrint(getWidth(), getHeight(), 0, 0);
    }

    private void resetPrint(final int width, final int height,
                            final int samplesPixel, final int samplesLight) {
        fpsT_ = String.format(Locale.US, "FPS:%.2f ", 0.0f);
        fps_ = 0.0f;
        fpsRenderT_ = String.format(Locale.US, "[%.2f]", fps_);
        timeFrameT_ = String.format(Locale.US, ", t:%.2fs ", 0.0f);
        timeT_ = String.format(Locale.US, "[%.2fs] ", 0.0f);
        stageT_ = " " + Stage.values()[stage_];
        allocatedT_ = ", Malloc:" + Debug.getNativeHeapAllocatedSize() / 1048576L + " MB";
        //availableT_ = "[Ma:" + Debug.getNativeHeapSize() / 1048576 + "MB";
        //freeT_ = ", Mf:" + Debug.getNativeHeapFreeSize() / 1048576 + "MB]";

        resolutionT_ = ",R:" + width + " x " + height;
        threadsT_ = ",T:" + numThreads_;
        samplesPixelT_ = ",spp:" + samplesPixel;
        samplesLightT_ = ",spl:" + samplesLight;
        sampleT_ = ", 0";
    }

    void setView(final TextView textView) {
        textView_ = textView;
        printText();
    }

    void FPS() {
        frame_++;
        final float time = SystemClock.elapsedRealtime();
        if ((time - timebase_) > 1000) {
            fps_ = (frame_ * 1000.0f) / (time - timebase_);
            timebase_ = time;
            frame_ = 0;
        }
    }

    private native void initialize(final int scene, final int shader, final int width,
                                   final int height, final int sampler, final int samplesPixel,
                                   final int samplesLight);

    private native void renderIntoBitmap(final Bitmap image, final int numThreads);

    native void finish();

    private native void stopRender();

    native void moveTouch(final float x, final float y, final int primitiveId);

    native int redraw(final Bitmap bitmap);

    native int traceTouch(final float x, final float y);

    native float getFPS();

    native long getTimeFrame();

    native int getSample();

    private native int resize(int size);

    native int isWorking();

    void stopDrawing() {
        this.setOnTouchListener(null);
        stopRender();
    }

    void startRender() {
        period_ = 200L;
        renderTask_ = new RenderTask();
        buttonRender_.setText(R.string.stop);
        start_ = SystemClock.elapsedRealtime();
        renderIntoBitmap(this.bitmap_, this.numThreads_);
        renderTask_.execute();
        this.setOnTouchListener(new TouchHandler());
    }

    void createScene(final int scene, final int shader, final int numThreads, final int sampler,
                     final int samplesPixel, final int samplesLight) {
        final int width = resize(getWidth());
        final int height = resize(getHeight());
        initialize(scene, shader, width, height, sampler, samplesPixel, samplesLight);
        numThreads_ = numThreads;
        frame_ = 0;
        timebase_ = 0.0f;

        bitmap_ = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        setBackground(new BitmapDrawable(getResources(), bitmap_));

        resetPrint(width, height, samplesPixel, samplesLight);
    }

    int getTouchListIndex(final int pointerID) {
        final int touchesSize = renderTask_.touches_.size();
        for (int i = 0; i < touchesSize; i++) {
            final TouchTracker thisTouch = renderTask_.touches_.get(i);
            if (pointerID == thisTouch.pointerID_) {
                return i;
            }
        }
        return -1;
    }

    void printText() {
        textView_.setText(
                fpsT_ + fpsRenderT_ + resolutionT_ + threadsT_ + samplesPixelT_ + samplesLightT_
                        + '\n'
                        + stageT_ + allocatedT_
                        //+ availableT_ + freeT_
                        + timeFrameT_ + timeT_ + sampleT_
        );
    }

    private enum Stage {
        IDLE(0), BUSY(1), END(2), STOP(3);
        final int id_;

        Stage(final int id) {
            this.id_ = id;
        }
    }

    final class RenderTask extends AsyncTask<Void, Void, Void> {
        final List<TouchTracker> touches_ = new ArrayList<>(1);
        private final Runnable timer_ = () -> {
            int touchesSize = touches_.size();
            for (int i = 0; i < touchesSize; i++) {
                final TouchTracker touch = touches_.get(i);
                moveTouch(touch.x_, touch.y_, touch.primitiveID_);
                /*System.out.println("[run," + Thread.currentThread().getId() + "]" + "moveTouch ("
                 + touch.x_ + "," + touch.y_ + ")");System.out.flush();*/
            }
            stage_ = redraw(bitmap_);
            if (stage_ != Stage.BUSY.id_) {
                scheduler_.shutdown();
            }
            FPS();
            fpsT_ = String.format(Locale.US, "FPS:%.1f ", getFPS());
            fpsRenderT_ = String.format(Locale.US, "[%.1f]", fps_);
            timeFrameT_ = String.format(Locale.US, ", t:%.2fs ", getTimeFrame() / 1000.0f);
            timeT_ = String.format(Locale.US, "[%.2fs] ",
                    (SystemClock.elapsedRealtime() - start_) / 1000.0f);
            stageT_ = " " + Stage.values()[stage_];
            allocatedT_ = ", Malloc:" + Debug.getNativeHeapAllocatedSize() / 1048576L + "MB";
            sampleT_ = ", " + getSample();
            //availableT_ = "[Ma:" + Debug.getNativeHeapSize() / 1048576 + "MB";
            //freeT_ = ", Mf:" + Debug.getNativeHeapFreeSize() / 1048576 + "MB]";
            publishProgress();
        };

        RenderTask() {
            super();
        }

        @Override
        protected final Void doInBackground(final Void... params) {
            scheduler_ = Executors.newSingleThreadScheduledExecutor();
            scheduler_.scheduleAtFixedRate(timer_, 0L, period_, TimeUnit.MILLISECONDS);
            boolean running = true;
            do {
                try {
                    running = !scheduler_.awaitTermination(2147483647L, TimeUnit.DAYS);
                } catch (final InterruptedException e) {
                    e.fillInStackTrace();
                }
            } while (running);
            return null;
        }

        @Override
        protected final void onProgressUpdate(final Void... progress) {
            printText();
        }

        @Override
        protected final void onPostExecute(final Void result) {
            redraw(bitmap_);
            printText();
            renderTask_.cancel(true);
            finish();
            buttonRender_.setText(R.string.render);
        }

        final class TouchTracker {
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

    private final class TouchHandler implements View.OnTouchListener {
        TouchHandler() {
            super();
        }

        @Override
        public final boolean onTouch(final View view, final MotionEvent motionEvent) {
            if (isWorking() != Stage.BUSY.id_) {
                return false;
            }
            switch (motionEvent.getActionMasked()) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN: {
                    final int actionIndex = motionEvent.getActionIndex();
                    final float x = motionEvent.getX(actionIndex);
                    final float y = motionEvent.getY(actionIndex);
                    final int primitiveID = traceTouch(x, y);
                    if (primitiveID < 0) {
                        return false;
                    }
                    final int pointerID = motionEvent.getPointerId(actionIndex);
                    final TouchTracker thisTouch = renderTask_.new TouchTracker(
                            pointerID, primitiveID, x, y);
                    renderTask_.touches_.add(thisTouch);
                    /*System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]"
                            + "ACTION_DOWN (" + x + "," + y + ")" + " ID:" + primitiveID);
                            System.out.flush();*/
                }
                return true;

                case MotionEvent.ACTION_MOVE:
                    final int pointerCount = motionEvent.getPointerCount();
                    for (int i = 0; i < pointerCount; i++) {
                        final int pointerID = motionEvent.getPointerId(i);
                        final int touchListIndex = getTouchListIndex(pointerID);
                        if (touchListIndex < 0) {
                            continue;
                        }
                        final TouchTracker touch = renderTask_.touches_.get(touchListIndex);
                        touch.x_ = motionEvent.getX(i);
                        touch.y_ = motionEvent.getY(i);
                        /*System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]"
                         + "ACTION_MOVE (" + touch.x_ + "," + touch.y_ + ")"
                         + " ID:" + touch.primitiveID_);System.out.flush();*/
                    }
                    return true;

                case MotionEvent.ACTION_POINTER_UP:
                    /*System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" +
                     "ACTION_POINTER_UP");System.out.flush();*/
                    final int actionIndex = motionEvent.getActionIndex();
                    final int pointerID = motionEvent.getPointerId(actionIndex);
                    final int touchListIndex = getTouchListIndex(pointerID);
                    if (touchListIndex < 0) {
                        return false;
                    }
                    renderTask_.touches_.remove(touchListIndex);
                    return true;

                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL:
                    /*System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" +
                     "ACTION_CANCEL");System.out.flush();*/
                    renderTask_.touches_.clear();
                    return true;

                default:
                    /*System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" +
                     "default");System.out.flush();*/
                    return false;
            }
        }
    }
}
