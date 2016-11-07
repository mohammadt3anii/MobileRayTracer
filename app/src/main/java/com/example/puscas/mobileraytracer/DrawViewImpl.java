package com.example.puscas.mobileraytracer;

import android.graphics.Bitmap;
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

    public DrawViewImpl() {
        width_ = 0;
        height_ = 0;
    }

    public void setResolution(int width, int height) {
        width_ = width;
        height_ = height;
        bitmap_ = Bitmap.createBitmap(width_, height_, Bitmap.Config.ARGB_8888);
    }

    public void initialize(int scene, int shader) {
        if (width_ > 0 && height_ > 0) {
            this.initialize(scene, shader, this.width_, this.height_);
        }
    }

    private native void initialize(int scene, int shader, int width, int height);

    private native void drawIntoBitmap(Bitmap image, int width);

    public void onDraw(Canvas canvas) {
        //Clear screen with background color

        bitmap_.eraseColor(Color.BLUE);

        long start = SystemClock.elapsedRealtime();

        // Call into our C++ code that renders to the bitmap
        //System.out.println(bitmap_.getWidth() + " " + bitmap_.getHeight());
        drawIntoBitmap(this.bitmap_, this.width_);

        long end = SystemClock.elapsedRealtime() - start;

        // Present the bitmap on the screen
        canvas.drawBitmap(bitmap_, 0.0f, 0.0f, null);
    }

    public int getWidth() {
        return width_;
    }

    public int getHeight() {
        return height_;
    }
}