package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Color;
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

    public DrawView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    }

    private native void initialize(int scene, int shader, int width, int height);

    private native void drawIntoBitmap(Bitmap image);

    private native int isWorking();

    private native void finished();

    public void createScene(int scene, int shader, TextView textView)
    {
        textView_ = textView;
        this.bitmapW_ = createBitmap(this.getWidth(), this.getHeight(), Config.ARGB_8888);
        this.initialize(scene, shader, this.getWidth(), this.getHeight());
        bitmapW_.eraseColor(Color.BLUE);
    }

    @Override
    public void onDraw(Canvas canvas)
    {
        if (this.isInEditMode() == false)
        {
            switch (this.isWorking()) {
                case 0://Iniciar renderizaçao
                    start_ = SystemClock.elapsedRealtime();
                    this.drawIntoBitmap(bitmapW_);//correr ray tracer no bitmapW
                    this.invalidate();
                    break;

                case 1://Enquanto ray-tracer ainda está a funcionar
                    bitmapR_ = createBitmap(bitmapW_);//copiar bitmap
                    canvas.drawBitmap(bitmapR_, 0.0f, 0.0f, null);//desenhar bitmapR
                    renderTime_ = SystemClock.elapsedRealtime() - start_;
                    textView_.setText("w:" + this.getWidth() + ", h:" + this.getHeight() + ", t:" + renderTime_ + " ms");
                    this.invalidate();
                    break;

                case 2://Quando ray-tracer acabar
                    renderTime_ = SystemClock.elapsedRealtime() - start_;
                    this.finished();
                    canvas.drawBitmap(bitmapW_, 0.0f, 0.0f, null);//desenhar bitmapW
                    textView_.setText("w:" + this.getWidth() + ", h:" + this.getHeight() + ", t:" + renderTime_ + " ms");
                    renderTime_ = 0;
                    break;

                default:
                    break;
            }
        }
    }
}