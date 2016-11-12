package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.TextView;

import com.example.puscas.mobileraytracer.R.id;
import com.example.puscas.mobileraytracer.R.layout;

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
    private MessageHandler handler_;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        this.setContentView(layout.activity_main);

        this.textView_ = (TextView) this.findViewById(id.timeText);
        this.drawView_ = (DrawView) this.findViewById(id.viewDraw);
        this.handler_ = new MessageHandler();
        this.drawView_.setHandler(this.handler_);
        this.mRenderButton_ = (Button) this.findViewById(id.renderButton);
        this.drawView_.setVisibility(View.INVISIBLE);
    }

    public void startRender(View view)
    {
        this.mRenderButton_.setEnabled(false);
        this.drawView_.createScene(this.scene_, this.shader_, this.textView_);
        this.drawView_.invalidate();
        this.drawView_.setVisibility(View.VISIBLE);
    }

    public void onSceneRadioButtonClicked(View view)
    {
        // Is the button now checked?
        if (((RadioButton) view).isChecked() == false) return;

        // Check which radio button was clicked
        switch (view.getId())
        {
            case id.radioCornell:
                scene_ = 0; // cornell
                break;

            case id.radioSpheres:
                scene_ = 1; // spheres
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
            case id.radioNoShadows:
                shader_ = 0; // No Shadows
                break;

            case id.radioWhitted:
                shader_ = 1; // Whitted
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
                    mRenderButton_.setEnabled(true);
                    break;

                default:
                    super.handleMessage(inputMessage);
                    break;
            }
        }
    }
}