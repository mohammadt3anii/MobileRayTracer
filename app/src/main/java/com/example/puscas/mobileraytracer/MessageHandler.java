package com.example.puscas.mobileraytracer;

import android.os.Handler;
import android.os.Message;

class MessageHandler extends Handler {
    private android.widget.Button mRenderButton_;

    MessageHandler(android.widget.Button button) {
        this.mRenderButton_ = button;
    }

    public void handleMessage(Message inputMessage) {
        switch (inputMessage.what) {
            case 1:        // Render finished
                this.mRenderButton_.setEnabled(true);
                break;

            default:
                super.handleMessage(inputMessage);
                break;
        }
    }
}