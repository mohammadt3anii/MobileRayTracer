package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;

public class DrawView extends View {

    private DrawViewImpl impl_;

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
                impl_ = new DrawViewImpl(getWidth(), getHeight());
            }
        });
    }

    @Override
    public void onDraw(Canvas canvas) {
        if(!isInEditMode())
        {
            //Call the implementation (it have the connection to c++)
            impl_.onDraw(canvas);

            //TODO:: need to call this when the user want or in 1 in 1 second
            //invalidate();
        }


    }
}