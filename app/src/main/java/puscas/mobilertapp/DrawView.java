package puscas.mobilertapp;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

public class DrawView extends GLSurfaceView {
    final ViewText viewText_ = new ViewText();
    MainRenderer renderer_;
    private Bitmap bitmap_ = null;
    private RenderTask renderTask_ = null;
    private int numThreads_ = 0;

    public DrawView(final Context context, final AttributeSet attrs) {
        super(context, attrs);
        viewText_.resetPrint(getWidth(), getHeight(), 0, 0);
    }

    native long initialize(final int scene, final int shader, final int width, final int height, final int accelerator, final int samplesPixel, final int samplesLight, final String objFile, final String matText, final AssetManager assetManager);

    native void renderIntoBitmap(final Bitmap image, final int numThreads);

    @Override
    public void onPause() {

    }

    @Override
    public void onResume() {

    }

    @Override
    public boolean performClick() {
        super.performClick();
        return true;
    }

    void setView(final TextView textView) {
        viewText_.textView_ = textView;
        viewText_.printText();
    }

    private native void stopRender();

    native int traceTouch(final float x, final float y);

    private native long getNumberOfLights();

    private native int resize(int size);


    void stopDrawing() {
        this.setOnTouchListener(null);
        stopRender();
    }

    void startRender() {
        viewText_.period_ = 250L;
        viewText_.buttonRender_.setText(R.string.stop);
        renderTask_ = new RenderTask(viewText_, bitmap_, () -> {
            requestRender();
            return 0;
        });
        viewText_.start_ = SystemClock.elapsedRealtime();
        renderIntoBitmap(bitmap_, numThreads_);
        renderTask_.execute();
        this.setOnTouchListener(new DrawView.TouchHandler());
    }

    void createScene(final int scene, final int shader, final int numThreads, final int accelerator,
                     final int samplesPixel, final int samplesLight, final float size, final String objFile, final String matText, final AssetManager assetManager) {
        setVisibility(View.INVISIBLE);
        final int width = resize(Math.round(getWidth() * size));
        final int height = resize(Math.round(getHeight() * size));
        viewText_.nPrimitivesT_ = ",p=" + initialize(scene, shader, width, height, accelerator, samplesPixel, samplesLight, objFile, matText, assetManager) + ",l=" + getNumberOfLights();
        numThreads_ = numThreads;
        viewText_.resetPrint(width, height, samplesPixel, samplesLight);
        bitmap_ = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        bitmap_.eraseColor(Color.DKGRAY);
        renderer_.bitmap_ = bitmap_;
        setVisibility(View.VISIBLE);
    }

    private int getTouchListIndex(final int pointerID) {
        final int touchesSize = renderTask_.touches_.size();
        for (int i = 0; i < touchesSize; i++) {
            final TouchTracker thisTouch = renderTask_.touches_.get(i);
            if (pointerID == thisTouch.pointerID_) {
                return i;
            }
        }
        return -1;
    }

    enum Stage {
        idle(0), busy(1), end(2), stop(3);
        final int id_;

        Stage(final int id) {
            this.id_ = id;
        }
    }

    private final class TouchHandler implements View.OnTouchListener {
        TouchHandler() {
            super();
        }

        @Override
        public final boolean onTouch(final View view, final MotionEvent motionEvent) {
            if (viewText_.isWorking() != Stage.busy.id_) {
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
                    final TouchTracker thisTouch = new TouchTracker(
                            pointerID, primitiveID, x, y);
                    renderTask_.touches_.add(thisTouch);
                    /*System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]"
                            + "ACTION_DOWN (" + x + "," + y + ")" + " ID:" + primitiveID);
                            System.out.flush();*/
                }
                break;

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
                    break;

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
                    break;

                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL:
                    /*System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" +
                     "ACTION_CANCEL");System.out.flush();*/
                    renderTask_.touches_.clear();
                    break;

                default:
                    /*System.out.println("[TouchHandler," + Thread.currentThread().getId() + "]" +
                     "default");System.out.flush();*/
                    return false;
            }
            performClick();
            return true;
        }
    }
}
