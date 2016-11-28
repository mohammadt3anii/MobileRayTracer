package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.os.Handler;
import android.os.Message;
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
    private Bitmap bitmapR_;
    private Handler handler_;
    private int numThreads_;

    public DrawView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    }

    private native void initialize(int scene, int shader, int width, int height, int sampler, int samples);
    private native void drawIntoBitmap(Bitmap image, int numThreads);
    private native int isWorking();
    private native void finished();

    public void setHandler(Handler pHandle) {
        handler_ = pHandle;
    }

    public void createScene(int scene, int shader, int numThreads, TextView textView, int sampler, int samples)
    {
        textView_ = textView;
        bitmapW_ = createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
        initialize(scene, shader, getWidth(), getHeight(), sampler, samples);
        bitmapW_.eraseColor(Color.WHITE);
        numThreads_ = numThreads;
    }

    public void onDraw(Canvas canvas)
    {
        if (!isInEditMode())
        {
            switch (isWorking()) {
                case 0://Start rendering
                {
                    start_ = SystemClock.elapsedRealtime();
                    drawIntoBitmap(bitmapW_, numThreads_);//run ray-tracer to bitmapW
                    invalidate();
                }
                    break;

                case 1://While ray-tracer is busy
                {
                    bitmapR_ = createBitmap(bitmapW_);//copy bitmap
                    canvas.drawBitmap(this.bitmapR_, 0.0f, 0.0f, null);//draw bitmapR
                    long renderTime = SystemClock.elapsedRealtime() - this.start_;
                    textView_.setText("Rendering -> " + getWidth() + "x" + getHeight() + ", T:" + this.numThreads_ + ", t:" + renderTime + "ms");
                    invalidate();
                }
                    break;

                case 2://When ray-tracer is finished
                {
                    long renderTime = SystemClock.elapsedRealtime() - this.start_;
                    finished();
                    canvas.drawBitmap(this.bitmapW_, 0.0f, 0.0f, null);//draw bitmapW
                    textView_.setText("Rendered -> " + getWidth() + "x" + getHeight() + ", T:" + this.numThreads_ + ", t:" + renderTime + "ms");
                    bitmapW_.recycle();
                    bitmapR_.recycle();
                    Message completeMessage = this.handler_.obtainMessage(1);
                    completeMessage.sendToTarget();
                }
                    break;

                default:
                    break;
            }
        }
    }
}