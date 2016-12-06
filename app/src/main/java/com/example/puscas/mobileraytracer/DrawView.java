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
    private Handler handler_;
    private int numThreads_;
    private int samples_;

    public DrawView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    }

    private native void initialize(int scene, int shader, int width, int height, int sampler, int samples);
    private native void drawIntoBitmap(Bitmap image, int numThreads);
    private native void finished();

    native void stopRender();

    native int isWorking();

    void setHandler(Handler pHandle) {
        handler_ = pHandle;
    }

    void createScene(int scene, int shader, int numThreads, TextView textView, int sampler, int samples)
    {
        textView_ = textView;
        bitmapW_ = createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
        initialize(scene, shader, getWidth(), getHeight(), sampler, samples);
        bitmapW_.eraseColor(Color.WHITE);
        numThreads_ = numThreads;
        samples_ = samples;
    }

    public void onDraw(Canvas canvas)
    {
        if (!isInEditMode())
        {
            String text = getWidth() + "x" + getHeight() + ", T:" + this.numThreads_ + ", S:" + this.samples_ + ", t:";
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
                    final Bitmap bitmapR = createBitmap(bitmapW_);
                    canvas.drawBitmap(bitmapR, 0.0f, 0.0f, null);
                    final long renderTime = SystemClock.elapsedRealtime() - this.start_;
                    textView_.setText("Rendering -> " + text + renderTime + "ms");
                    invalidate();
                }
                    break;

                case 2://When ray-tracer is finished
                {
                    final long renderTime = SystemClock.elapsedRealtime() - this.start_;
                    finished();
                    canvas.drawBitmap(this.bitmapW_, 0.0f, 0.0f, null);
                    textView_.setText("Rendered -> " + text + renderTime + "ms");
                    bitmapW_.recycle();
                    final Message completeMessage = this.handler_.obtainMessage(1);
                    completeMessage.sendToTarget();
                }
                    break;

                default:
                    break;
            }
        }
    }
}