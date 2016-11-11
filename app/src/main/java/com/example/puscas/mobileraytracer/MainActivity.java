package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.TextView;

public class MainActivity extends Activity
{
    static {
        System.loadLibrary("DrawView");
    }

    private Button mRenderButton_;
    private TextView textView_;
    private DrawView drawView_;
    private int scene_;
    private int shader_;
    private MainActivity.MessageHandler handler_;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView_ = (TextView) findViewById(R.id.timeText);
        drawView_ = (DrawView) findViewById(R.id.viewDraw);
        handler_ = new MainActivity.MessageHandler();
        drawView_.setHandler(handler_);
        mRenderButton_ = (Button) findViewById(R.id.renderButton);
        drawView_.setVisibility(View.INVISIBLE);
    }

    public void startRender(View view)
    {
        mRenderButton_.setEnabled(false);
        drawView_.createScene(scene_, shader_, textView_);
        drawView_.invalidate();
        drawView_.setVisibility(View.VISIBLE);
    }

    public void onSceneRadioButtonClicked(View view)
    {
        // Is the button now checked?
        if (((RadioButton) view).isChecked() == false) return;

        // Check which radio button was clicked
        switch (view.getId())
        {
            case R.id.radioCornell:
                this.scene_ = 0; // cornell
                break;

            case R.id.radioSpheres:
                this.scene_ = 1; // spheres
                break;

            default:
                break;
        }
    }

    public void onShaderRadioButtonClicked(View view)
    {
        // Is the button now checked?
        if (((RadioButton) view).isChecked() == false) return;

        // Check which radio button was clicked
        switch (view.getId())
        {
            case R.id.radioNoShadows:
                this.shader_ = 0; // No Shadows
                break;

            case R.id.radioWhitted:
                this.shader_ = 1; // Whitted
                break;

            default:
                break;
        }
    }

    private class MessageHandler extends Handler {
        @Override
        public void handleMessage(Message inputMessage) {
            switch (inputMessage.what) {
                case 1:        // Render finished
                    MainActivity.this.mRenderButton_.setEnabled(true);
                    break;

                default:
                    super.handleMessage(inputMessage);
                    break;
            }
        }
    }
}