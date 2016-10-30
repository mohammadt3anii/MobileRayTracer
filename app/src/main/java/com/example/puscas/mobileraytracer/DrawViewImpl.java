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
    private native void initialize(int scene, int shader, int width, int height);
    private native void drawIntoBitmap(Bitmap image, int width, int height, long elapsedTime);

    private int width_ = 0;
    private int height_ = 0;
    private Bitmap bitmap_;

    public DrawViewImpl( int width, int height) {
        width_ = width;
        height_ = height;

        initialize( 1, 1, width, height);
    }

    public void onDraw(Canvas canvas) {
        //Clear screen with background color
        bitmap_ = Bitmap.createBitmap( width_, height_, Bitmap.Config.ARGB_8888);
        bitmap_.eraseColor(Color.BLUE);

        long start = SystemClock.elapsedRealtime();

        // Call into our C++ code that renders to the bitmap
        //System.out.println(bitmap_.getWidth() + " " + bitmap_.getHeight());
        drawIntoBitmap(bitmap_, width_, height_, SystemClock.elapsedRealtime());

        long end = SystemClock.elapsedRealtime() - start;
        System.out.println(end);

        // Present the bitmap on the screen
        canvas.drawBitmap(bitmap_, 0.0f, 0.0f, null);

    }

}
