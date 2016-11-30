package com.example.puscas.mobileraytracer;

import android.os.Handler;
import android.os.Message;

class MessageHandler extends Handler {
    private android.widget.Button mRenderButton_;
    private MainActivity mainActivity_;

    MessageHandler(MainActivity mainActivity, android.widget.Button button) {
        this.mainActivity_ = mainActivity;
        this.mRenderButton_ = button;
    }

    public void handleMessage(Message inputMessage) {
        switch (inputMessage.what) {
            case 1:        // Render finished
                this.mRenderButton_.setText(mainActivity_.getString(R.string.render));
                break;

            case 2:        // Render stopped
                this.mRenderButton_.setText(mainActivity_.getString(R.string.render));
                break;

            default:
                super.handleMessage(inputMessage);
                break;
        }
    }
}