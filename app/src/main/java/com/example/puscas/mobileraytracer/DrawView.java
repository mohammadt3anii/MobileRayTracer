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

/**
 * Created by Tiago on 09/10/2016.
 */

public class DrawView extends View
{
    private long start_;
    private long renderTime_;
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

    @Override
    public void onDraw(Canvas canvas)
    {
        if (isInEditMode() == false)
        {
            switch (isWorking()) {
                case 0://Iniciar renderizaçao
                    start_ = SystemClock.elapsedRealtime();
                    drawIntoBitmap(bitmapW_, numThreads_);//correr ray tracer no bitmapW
                    invalidate();
                    break;

                case 1://Enquanto ray-tracer ainda está a funcionar
                    bitmapR_ = createBitmap(bitmapW_);//copiar bitmap
                    canvas.drawBitmap(this.bitmapR_, 0.0f, 0.0f, null);//desenhar bitmapR
                    renderTime_ = SystemClock.elapsedRealtime() - this.start_;
                    textView_.setText("Rendering -> " + getWidth() + "x" + getHeight() + ", T:" + this.numThreads_ + ", t:" + renderTime_ + "ms");
                    invalidate();
                    break;

                case 2://Quando ray-tracer acabar
                    renderTime_ = SystemClock.elapsedRealtime() - this.start_;
                    finished();
                    canvas.drawBitmap(this.bitmapW_, 0.0f, 0.0f, null);//desenhar bitmapW
                    textView_.setText("Rendered -> " + getWidth() + "x" + getHeight() + ", T:" + this.numThreads_ + ", t:" + renderTime_ + "ms");
                    renderTime_ = 0;
                    bitmapW_.recycle();
                    bitmapR_.recycle();
                    Message completeMessage = this.handler_.obtainMessage(1);
                    completeMessage.sendToTarget();
                    break;

                default:
                    break;
            }
        }
    }
}