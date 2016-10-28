package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;

public class DrawView extends View {


    //static {
    //    System.loadLibrary("DrawView");
    //}
    //private native void drawIntoBitmap(Bitmap image, int width, int height, long elapsedTime);

    //private Bitmap bitmap_;

    DrawViewImpl impl_;

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
                //bitmap_ = Bitmap.createBitmap( , Bitmap.Config.ARGB_8888);
                impl_ = new DrawViewImpl(getWidth(), getHeight());
            }
        });
    }

    @Override
    public void onDraw(Canvas canvas) {
        if(!isInEditMode())
        {
            impl_.onDraw(canvas);
        }
        //super.onDraw(canvas);
        //Clear screen with background color
        //Bitmap bitmap = Bitmap.createBitmap( getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
        //bitmap.eraseColor(Color.BLUE);

        // Call into our C++ code that renders to the bitmap
        //System.out.println(bitmap_.getWidth() + " " + bitmap_.getHeight());

        //drawIntoBitmap(bitmap, bitmap.getWidth(), bitmap.getHeight(), SystemClock.elapsedRealtime());

        // Present the bitmap on the screen
        //canvas.drawBitmap(bitmap, 0, 0, null);

        //TODO:: need to call this when the user want or in 1 in 1 second
        //invalidate();
    }
}