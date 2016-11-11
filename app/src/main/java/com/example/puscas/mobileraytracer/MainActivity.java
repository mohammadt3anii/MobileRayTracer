package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Bundle;
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

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        this.setContentView(layout.activity_main);

        textView_ = (TextView) this.findViewById(id.timeText);
        drawView_ = (DrawView) this.findViewById(id.viewDraw);
        mRenderButton_ = (Button) this.findViewById(id.renderButton);

        drawView_.setVisibility(View.INVISIBLE);
    }

    public void startRender(View view)
    {
        mRenderButton_.setEnabled(false);
        drawView_.invalidate();
        drawView_.createScene(scene_, shader_, textView_);
        drawView_.setVisibility(View.VISIBLE);
        mRenderButton_.setEnabled(true);
    }

    public void onSceneRadioButtonClicked(View view)
    {
        // Is the button now checked?
        boolean checked = ((RadioButton) view).isChecked();

        // Check which radio button was clicked
        switch (view.getId())
        {
            case id.radioCornell:
                if (checked)
                {
                    scene_ = 0; // cornell
                }
                break;

            case id.radioSpheres:
                if (checked)
                {
                    scene_ = 1; // spheres
                }
                break;

            default:
                break;
        }
    }

    public void onShaderRadioButtonClicked(View view)
    {
        // Is the button now checked?
        boolean checked = ((RadioButton) view).isChecked();

        // Check which radio button was clicked
        switch (view.getId())
        {
            case id.radioNoShadows:
                if (checked)
                {
                    shader_ = 0; // No Shadows
                }
                break;

            case id.radioWhitted:
                if (checked)
                {
                    shader_ = 1; // Whitted
                }
                break;

            default:
                break;
        }
    }
}