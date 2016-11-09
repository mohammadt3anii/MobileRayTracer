package com.example.puscas.mobileraytracer;

import android.content.Context;
import android.graphics.Canvas;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.view.View;
import android.widget.TextView;

public class DrawView extends View
{
    private DrawViewImpl impl_;
    private long lastRenderTime_;
    private TextView textView_;
    private int scene_;
    private int shader_;

    public DrawView(Context context)
    {
        super(context);
        this.init(context);
    }

    public DrawView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        this.init(context);
    }

    public DrawView(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
        this.init(context);
    }

    public void createScene(int scene, int shader, TextView textView)
    {
        this.scene_ = scene;
        this.shader_ = shader;
        this.lastRenderTime_ = 0;
        this.textView_ = textView;
        if (this.impl_ == null)
        {
            this.impl_ = new DrawViewImpl();
        }
        if (getWidth() != this.impl_.getWidth() ||
                getHeight() != this.impl_.getHeight())
        {
            this.impl_.setResolution(getWidth(), getHeight());
        }
        this.impl_.initialize(this.scene_, this.shader_);
    }

    private void init(Context context)
    {

    }

    @Override
    public void onDraw(Canvas canvas)
    {
        if (isInEditMode() == false)
        {
            long start = SystemClock.elapsedRealtime();

            //Call the implementation (it has the connection to c++)
            this.impl_.onDraw(canvas);

            this.lastRenderTime_ = SystemClock.elapsedRealtime() - start;
            this.textView_.setText("w:" + getWidth() + ", h:" + getHeight() + ", t:" + this.lastRenderTime_ + " ms");
        }
    }
}