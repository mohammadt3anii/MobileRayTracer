package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Canvas;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.view.View;
import android.widget.TextView;

public class DrawView extends View {

    private DrawViewImpl impl_;
    private long lastRenderTime_;
    private TextView textView_;

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
        lastRenderTime_ = 0;
        textView_ = null;
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
            long start = SystemClock.elapsedRealtime();

            //Call the implementation (it have the connection to c++)
            impl_.onDraw(canvas);

            lastRenderTime_ = SystemClock.elapsedRealtime() - start;
            if(textView_ != null)
            {
                textView_.setText("w:"+getWidth()+", h:"+getHeight()+", t:"+lastRenderTime_+" ms");
            }




            //TODO:: need to call this when the user want or in 1 in 1 second
            //invalidate();
        }
    }

    public void setTextView(TextView textView)
    {
        textView_ = textView;
    }

    public long getLastRenderTime()
    {
        return lastRenderTime_;
    }
}