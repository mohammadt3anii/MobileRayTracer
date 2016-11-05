package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.TextView;

import com.example.puscas.mobileraytracer.R.id;
import com.example.puscas.mobileraytracer.R.layout;

public class MainActivity extends Activity {

    private Button mRenderButton;
    private TextView textView;
    private DrawView drawView;
    private int scene_;
    private int shader_;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.setContentView(layout.activity_main);

        this.textView = (TextView) this.findViewById(id.timeText);
        this.drawView = (DrawView) this.findViewById(id.viewDraw);
        this.mRenderButton = (Button) this.findViewById(id.renderButton);

        this.drawView.setVisibility(View.INVISIBLE);
    }

    public void startRender(View view) {
        this.mRenderButton.setEnabled(false);
        System.out.println("scene = " + this.scene_);
        this.drawView.invalidate();
        this.drawView.createScene(this.scene_, this.shader_, this.textView);
        this.drawView.setVisibility(View.VISIBLE);
        this.mRenderButton.setEnabled(true);
    }

    public void onSceneRadioButtonClicked(View view) {
        // Is the button now checked?
        boolean checked = ((RadioButton) view).isChecked();

        // Check which radio button was clicked
        switch (view.getId()) {
            case id.radioCornell:
                if (checked)
                    this.scene_ = 0; // cornell
                break;

            case id.radioSpheres:
                if (checked)
                    this.scene_ = 1; // spheres
                break;
        }
    }

    public void onShaderRadioButtonClicked(View view) {
        // Is the button now checked?
        boolean checked = ((RadioButton) view).isChecked();

        // Check which radio button was clicked
        switch (view.getId()) {
            case id.radioNoShadows:
                if (checked)
                    this.shader_ = 0; // No Shadows
                break;

            case id.radioWhitted:
                if (checked)
                    this.shader_ = 1; // Whitted
                break;
        }
    }
}