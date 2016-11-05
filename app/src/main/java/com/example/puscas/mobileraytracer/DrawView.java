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
    private int scene_;
    private int shader_;

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

    public void createScene(int scene, int shader, TextView textView) {
        scene_ = scene;
        shader_ = shader;
        lastRenderTime_ = 0;
        textView_ = textView;
        if (impl_ == null) {
            impl_ = new DrawViewImpl();
        }
        if (getWidth() != impl_.getWidth() ||
                getHeight() != impl_.getHeight()) {
            impl_.setResolution(getWidth(), getHeight());
        }
        impl_.initialize(scene_, shader_);
    }

    private void init(Context context) {

    }

    @Override
    public void onDraw(Canvas canvas) {
        if (!isInEditMode())
        {
            long start = SystemClock.elapsedRealtime();

            //Call the implementation (it has the connection to c++)
            impl_.onDraw(canvas);

            lastRenderTime_ = SystemClock.elapsedRealtime() - start;
            if (textView_ != null)
            {
                textView_.setText("w:" + getWidth() + ", h:" + getHeight() + ", t:" + lastRenderTime_ + " ms");
            }
        }
    }

    public long getLastRenderTime()
    {
        return lastRenderTime_;
    }
}