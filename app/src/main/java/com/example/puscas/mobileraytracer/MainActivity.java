package com.example.puscas.mobileraytracer;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends Activity {



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView textView = (TextView) findViewById(R.id.timeText);
        DrawView drawView = (DrawView) findViewById(R.id.viewDraw);
        drawView.setTextView(textView);
    }
}