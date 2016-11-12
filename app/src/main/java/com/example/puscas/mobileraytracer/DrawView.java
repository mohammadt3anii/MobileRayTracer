package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
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
    private long renderTime_;
    private TextView textView_;
    private Bitmap bitmapW_;
    private Bitmap bitmapR_;
    private Handler handler_;

    public DrawView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    }

    private native void initialize(int scene, int shader, int width, int height);
    private native void drawIntoBitmap(Bitmap image);
    private native int isWorking();
    private native void finished();

    public void setHandler(Handler pHandle) {
        this.handler_ = pHandle;
    }

    public void createScene(int scene, int shader, TextView textView)
    {
        this.textView_ = textView;
        this.bitmapW_ = createBitmap(this.getWidth(), this.getHeight(), Config.ARGB_8888);
        this.initialize(scene, shader, this.getWidth(), this.getHeight());
        this.bitmapW_.eraseColor(Color.BLUE);
    }

    @Override
    public void onDraw(Canvas canvas)
    {
        if (this.isInEditMode() == false)
        {
            switch (this.isWorking()) {
                case 0://Iniciar renderizaçao
                    this.start_ = SystemClock.elapsedRealtime();
                    this.drawIntoBitmap(this.bitmapW_);//correr ray tracer no bitmapW
                    this.invalidate();
                    break;

                case 1://Enquanto ray-tracer ainda está a funcionar
                    this.bitmapR_ = createBitmap(this.bitmapW_);//copiar bitmap
                    canvas.drawBitmap(bitmapR_, 0.0f, 0.0f, null);//desenhar bitmapR
                    this.renderTime_ = SystemClock.elapsedRealtime() - start_;
                    this.textView_.setText("Rendering -> w:" + this.getWidth() + ", h:" + this.getHeight() + ", t:" + this.renderTime_ + " ms");
                    this.invalidate();
                    break;

                case 2://Quando ray-tracer acabar
                    this.renderTime_ = SystemClock.elapsedRealtime() - start_;
                    this.finished();
                    canvas.drawBitmap(bitmapW_, 0.0f, 0.0f, null);//desenhar bitmapW
                    this.textView_.setText("Rendered -> w:" + this.getWidth() + ", h:" + this.getHeight() + ", t:" + this.renderTime_ + " ms");
                    this.renderTime_ = 0;
                    this.bitmapW_.recycle();
                    this.bitmapR_.recycle();
                    Message completeMessage = handler_.obtainMessage(1);
                    completeMessage.sendToTarget();
                    break;

                default:
                    break;
            }
        }
    }
}