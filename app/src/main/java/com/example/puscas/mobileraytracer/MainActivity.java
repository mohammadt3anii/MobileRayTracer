package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.TextView;

public class MainActivity extends Activity {

    private Button mRenderButton;
    private TextView textView;
    private DrawView drawView;
    private int scene_;
    private int shader_;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView = (TextView) findViewById(R.id.timeText);
        drawView = (DrawView) findViewById(R.id.viewDraw);
        mRenderButton = (Button) findViewById(R.id.renderButton);

        drawView.setVisibility(View.INVISIBLE);
    }

    public void startRender(View view) {
        mRenderButton.setEnabled(false);
        drawView.invalidate();
        drawView.createScene(scene_, shader_, textView);
        drawView.setVisibility(View.VISIBLE);
        mRenderButton.setEnabled(true);
    }

    public void onSceneRadioButtonClicked(View view) {
        // Is the button now checked?
        boolean checked = ((RadioButton) view).isChecked();

        // Check which radio button was clicked
        switch (view.getId()) {
            case R.id.radioCornell:
                if (checked) {
                    scene_ = 0; // cornell
                }
                break;

            case R.id.radioSpheres:
                if (checked) {
                    scene_ = 1; // spheres
                }
                break;

            default:
                break;
        }
    }

    public void onShaderRadioButtonClicked(View view) {
        // Is the button now checked?
        boolean checked = ((RadioButton) view).isChecked();

        // Check which radio button was clicked
        switch (view.getId()) {
            case R.id.radioNoShadows:
                if (checked) {
                    shader_ = 0; // No Shadows
                }
                break;

            case R.id.radioWhitted:
                if (checked) {
                    shader_ = 1; // Whitted
                }
                break;
            default:
                break;
        }
    }
}