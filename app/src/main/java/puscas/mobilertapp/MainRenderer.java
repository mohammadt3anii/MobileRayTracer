package puscas.mobilertapp;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView.Renderer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MainRenderer implements Renderer {
    private Triangle triangle;

    @Override
    public void onDrawFrame(final GL10 arg0) {
        GLES20.glClearColor(0.8f, 0.0f, 0.0f, 0.0f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
        triangle.draw();
    }

    @Override
    public void onSurfaceChanged(final GL10 arg0, final int arg1, final int arg2) {

    }

    @Override
    public void onSurfaceCreated(final GL10 arg0, final EGLConfig arg1) {
        triangle = new Triangle();
    }
}
