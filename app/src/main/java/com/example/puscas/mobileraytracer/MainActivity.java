package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.TextView;

public class MainActivity extends Activity
{
    private Button mRenderButton_;
    private TextView textView_;
    private DrawView drawView_;
    private int scene_;
    private int shader_;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        this.textView_ = (TextView) findViewById(R.id.timeText);
        this.drawView_ = (DrawView) findViewById(R.id.viewDraw);
        this.mRenderButton_ = (Button) findViewById(R.id.renderButton);

        this.drawView_.setVisibility(View.INVISIBLE);
    }

    public void startRender(View view)
    {
        this.mRenderButton_.setEnabled(false);
        this.drawView_.invalidate();
        this.drawView_.createScene(this.scene_, this.shader_, this.textView_);
        this.drawView_.setVisibility(View.VISIBLE);
        this.mRenderButton_.setEnabled(true);
    }

    public void onSceneRadioButtonClicked(View view)
    {
        // Is the button now checked?
        final boolean checked = ((RadioButton) view).isChecked();

        // Check which radio button was clicked
        switch (view.getId())
        {
            case R.id.radioCornell:
                if (checked)
                {
                    this.scene_ = 0; // cornell
                }
                break;

            case R.id.radioSpheres:
                if (checked)
                {
                    this.scene_ = 1; // spheres
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
            case R.id.radioNoShadows:
                if (checked)
                {
                    this.shader_ = 0; // No Shadows
                }
                break;

            case R.id.radioWhitted:
                if (checked)
                {
                    this.shader_ = 1; // Whitted
                }
                break;

            default:
                break;
        }
    }
}