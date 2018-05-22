package puscas.mobilertapp;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

public class DrawView extends GLSurfaceView {
    final ViewText viewText_ = new ViewText();
    MainRenderer renderer_ = null;
    private RenderTask renderTask_ = null;
    private int numThreads_ = 0;

    public DrawView(final Context context, final AttributeSet attrs) {
        super(context, attrs);
        viewText_.resetPrint(getWidth(), getHeight(), 0, 0, 0);
    }

    static native int initialize(final int scene, final int shader, final int width, final int height, final int accelerator, final int samplesPixel, final int samplesLight, final String objFile, final String matText, final AssetManager assetManager);

    static private native void renderIntoBitmap(final Bitmap image, final int numThreads, final boolean async);

    static private native float[] initVerticesArray();

    static private native float[] initColorsArray();

    static private native float[] initCameraArray();

    static private native void stopRender();

    static native int traceTouch(final float x, final float y);

    static private native int getNumberOfLights();

    static native int resize(int size);

    static native void finishRender();

    static private int calledByJNI_static() {
        //System.out.println("JNI1 CALLED THIS");
        return 0;
    }

    @Override
    public void onPause() {
        super.onPause();
    }

    @Override
    public void onResume() {
        super.onResume();
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

    private int calledByJNI() {
        //System.out.println("JNI2 CALLED THIS");
        return 0;
    }


    void stopDrawing() {
        this.setOnTouchListener(null);
        stopRender();
    }

    void startRender() {
        viewText_.period_ = 250;
        viewText_.buttonRender_.setText(R.string.stop);
        viewText_.start_ = (int) SystemClock.elapsedRealtime();
        viewText_.printText();
        renderTask_ = new RenderTask(viewText_, this::requestRender);

        try {
            Thread.sleep(1);
        } catch (InterruptedException e) {
            Log.e("MobileRT", e.getMessage());
        }

        queueEvent(() -> {
            final float[] arrayVertices = initVerticesArray();
            final float[] arrayColors = initColorsArray();
            final float[] arrayCamera = initCameraArray();

            if (arrayVertices != null) {
                renderer_.copyFrame(arrayVertices, arrayColors, arrayCamera);
            }
            DrawView.renderIntoBitmap(renderer_.bitmap_, numThreads_, true);

            renderTask_.execute();
            this.setOnTouchListener(new DrawView.TouchHandler());
        });
    }

    void createScene(final int scene, final int shader, final int numThreads, final int accelerator,
                     final int samplesPixel, final int samplesLight, final int width, final int height,
                     final String objFile, final String matText, final AssetManager assetManager) {
        viewText_.resetPrint(width, height, numThreads, samplesPixel, samplesLight);
        final int numberPrimitives = initialize(scene, shader, width, height, accelerator, samplesPixel, samplesLight, objFile, matText, assetManager);
        viewText_.nPrimitivesT_ = ",p=" + numberPrimitives + ",l=" + getNumberOfLights();
        numThreads_ = numThreads;

        setVisibility(View.INVISIBLE);
        renderer_.setBitmap(width, height, getWidth(), getHeight());
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
            if (ViewText.isWorking() != Stage.busy.id_) {
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
