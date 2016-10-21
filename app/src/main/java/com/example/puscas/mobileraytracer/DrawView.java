package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.view.View;

public class DrawView extends View {

    // Used to load the 'DrawView' library on application startup.
    static {
        System.loadLibrary("DrawView");
    }

    Bitmap bitmap_;

    public DrawView(Context context) {
        super(context);
        init(context);
    }

    public DrawView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public DrawView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(context);
    }

    private void init(Context context) {
        post(new Runnable() {
            @Override
            public void run() {
                bitmap_ = Bitmap.createBitmap( getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
            }
        });
    }

    @Override
    public void onDraw(Canvas canvas) {
        //Clear screen with background color
        bitmap_.eraseColor(Color.BLUE);

        // Call into our C++ code that renders to the bitmap
        System.out.println(bitmap_.getWidth() + " " + bitmap_.getHeight());
        drawIntoBitmap(bitmap_, bitmap_.getWidth(), bitmap_.getHeight(), SystemClock.elapsedRealtime());

        // Present the bitmap on the screen
        canvas.drawBitmap(bitmap_, 0, 0, null);

        //TODO:: We need to call this when the user whant or in 1 in 1 second
        invalidate();
    }

    private native void drawIntoBitmap(Bitmap image, int width, int height, long elapsedTime);

}