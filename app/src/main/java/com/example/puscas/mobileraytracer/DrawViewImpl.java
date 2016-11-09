package com.example.puscas.mobileraytracer;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;

/**
 * Created by Tiago on 28/10/2016.
 */

public class DrawViewImpl
{
    static
    {
        System.loadLibrary("DrawView");
    }

    private native void initialize(int scene, int shader, int width, int height);
    private native void drawIntoBitmap(Bitmap image, int width);

    private int width_;
    private int height_;
    private Bitmap bitmap_;

    public DrawViewImpl()
    {
        this.width_ = 0;
        this.height_ = 0;
    }

    public void setResolution(int width, int height)
    {
        width_ = width;
        height_ = height;
        bitmap_ = Bitmap.createBitmap(width_, height_, Bitmap.Config.ARGB_8888);
    }

    public void initialize(int scene, int shader)
    {
        if (this.width_ > 0 && this.height_ > 0)
        {
            initialize(scene, shader, this.width_, this.height_);
        }
    }

    public void onDraw(Canvas canvas)
    {
        //Clear screen with background color
        this.bitmap_.eraseColor(Color.BLUE);

        // Call into our C++ code that renders to the bitmap
        //System.out.println(bitmap_.getWidth() + " " + bitmap_.getHeight());
        drawIntoBitmap(this.bitmap_, this.width_);

        // Present the bitmap on the screen
        canvas.drawBitmap(this.bitmap_, 0.0f, 0.0f, null);
    }

    public int getWidth()
    {
        return this.width_;
    }

    public int getHeight()
    {
        return this.height_;
    }
}