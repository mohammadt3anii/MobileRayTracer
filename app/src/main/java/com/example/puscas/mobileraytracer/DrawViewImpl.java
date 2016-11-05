package com.example.puscas.mobileraytracer;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Color;
import android.os.SystemClock;

/**
 * Created by Tiago on 28/10/2016.
 */

public class DrawViewImpl
{
    static {
        System.loadLibrary("DrawView");
    }

    private int width_;
    private int height_;
    private Bitmap bitmap_;

    public DrawViewImpl(int width, int height, int scene, int shader) {
        this.width_ = width;
        this.height_ = height;

        this.initialize(scene, shader, width, height);
    }

    private native void initialize(int scene, int shader, int width, int height);

    private native void drawIntoBitmap(Bitmap image, int width, int height, long elapsedTime);

    public void onDraw(Canvas canvas) {
        //Clear screen with background color
        this.bitmap_ = Bitmap.createBitmap(this.width_, this.height_, Config.ARGB_8888);
        this.bitmap_.eraseColor(Color.BLUE);

        long start = SystemClock.elapsedRealtime();

        // Call into our C++ code that renders to the bitmap
        //System.out.println(bitmap_.getWidth() + " " + bitmap_.getHeight());
        this.drawIntoBitmap(this.bitmap_, this.width_, this.height_, SystemClock.elapsedRealtime());

        long end = SystemClock.elapsedRealtime() - start;
        System.out.println(end);

        // Present the bitmap on the screen
        canvas.drawBitmap(this.bitmap_, 0.0f, 0.0f, null);

    }

}
