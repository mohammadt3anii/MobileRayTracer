package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Handler;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.view.View;
import android.widget.TextView;

import static android.graphics.Bitmap.createBitmap;

public class DrawView extends View
{
    private long start_;
    private TextView textView_;
    private Bitmap bitmapW_;
    private Handler handler_;
    private int numThreads_;
    private String text_;

    public DrawView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    }

    private native void initialize(int scene, int shader, int width, int height, int sampler, int samples);
    private native void drawIntoBitmap(Bitmap image, int numThreads);
    private native void finish();
    native void stopRender();
    native int isWorking();

    void setHandler(Handler pHandle) {
        handler_ = pHandle;
    }

    void createScene(int scene, int shader, int numThreads, TextView textView, int sampler, int samples)
    {
        int width = getWidth();
        int height = getHeight();
        textView_ = textView;
        bitmapW_ = createBitmap(width, height, Bitmap.Config.ARGB_8888).copy(Bitmap.Config.ARGB_8888, true);
        initialize(scene, shader, width, height, sampler, samples);
        numThreads_ = numThreads;
        text_ = width + "x" + height + ", T:" + this.numThreads_ + ", S:" + samples + ", t:";
    }

    public void onDraw(Canvas canvas)
    {
        if (!isInEditMode())
        {
            switch (isWorking()) {
                case 0://Start rendering
                {
                    start_ = SystemClock.elapsedRealtime();
                    drawIntoBitmap(bitmapW_, numThreads_);
                    invalidate();
                }
                    break;

                case 1://While ray-tracer is busy
                {
                    canvas.drawBitmap(createBitmap(this.bitmapW_), 0.0f, 0.0f, null);
                    textView_.setText("Rendering -> " + text_ + (SystemClock.elapsedRealtime() - this.start_) + "ms");
                    invalidate();
                }
                    break;

                case 2://When ray-tracer is finished
                {
                    canvas.drawBitmap(this.bitmapW_, 0.0f, 0.0f, null);
                    textView_.setText("Rendered -> " + text_ + (SystemClock.elapsedRealtime() - this.start_) + "ms");
                    this.handler_.obtainMessage(1).sendToTarget();
                    finish();
                }
                break;

                case 3://When ray-tracer is stopped
                {
                    canvas.drawBitmap(this.bitmapW_, 0.0f, 0.0f, null);
                    textView_.setText("Stopped -> " + text_ + (SystemClock.elapsedRealtime() - this.start_) + "ms");
                    this.handler_.obtainMessage(1).sendToTarget();
                    finish();
                }
                    break;

                default:
                    break;
            }
        }
    }
}