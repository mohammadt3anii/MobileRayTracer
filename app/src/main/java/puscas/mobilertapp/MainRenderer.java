package puscas.mobilertapp;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLUtils;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MainRenderer implements Renderer {
    private final float[] vertices = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
    };
    private final float[] texCoords = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };
    String vertexShaderCode = null;
    String fragmentShaderCode = null;
    Bitmap bitmap_ = null;
    private FloatBuffer floatBufferVertices_ = null;
    private FloatBuffer floatBufferTexture_ = null;
    public int frame_ = 0;
    private int width_ = 1;
    private int height_ = 1;
    private int x_ = 0;
    private int y_ = 0;

    private static int loadShader(final int shaderType, final String source) {
        final int shader = GLES20.glCreateShader(shaderType);
        if (shader != 0) {
            GLES20.glShaderSource(shader, source);
            GLES20.glCompileShader(shader);
            final int[] compiled = new int[1];
            GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
            if (compiled[0] == 0) {
                Log.e("SHADER", "Could not compile shader " + shaderType + ':');
                Log.e("SHADER", GLES20.glGetShaderInfoLog(shader));
                Log.e("SHADER", source);
                GLES20.glDeleteShader(shader);
                System.exit(0);
            }
        }
        return shader;
    }

    private void checkGLError() {
        final int glError = GLES20.glGetError();
        if (glError != GLES20.GL_NO_ERROR) {
            Log.e("glError", "glError = " + GLUtils.getEGLErrorString(glError));
            System.exit(1);
        }
    }

    public void setBitmap(final int width, final int height, final int x, final int y) {
        bitmap_ = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        bitmap_.eraseColor(Color.BLACK);
        width_ = width;
        height_ = height;
        x_ = x;
        y_ = y;
        frame_ = 0;
    }

    private void copyBuffer() {
        int b[] = new int[width_ * (y_ + height_)];
        int bt[] = new int[width_ * height_];
        IntBuffer ib = IntBuffer.wrap(b);
        ib.position(0);

        GLES20.glReadPixels(x_, y_, width_, height_, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, ib);
        checkGLError();

        //remember, that OpenGL bitmap is incompatible with Android bitmap
        //and so, some correction need.
        for (int i = 0, k = 0; i < height_; i++, k++) {
            for (int j = 0; j < width_; j++) {
                int pixel = b[i * width_ + j];
                int blue = (pixel >> 16) & 0xff;
                int red = (pixel << 16) & 0x00ff0000;
                int pix1 = (pixel & 0xff00ff00) | red | blue;
                bt[(height_ - k - 1) * width_ + j] = pix1;
            }
        }

        bitmap_ = Bitmap.createBitmap(bt, width_, height_, Bitmap.Config.ARGB_8888);
    }

    @Override
    public void onDrawFrame(final GL10 gl) {
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, 4);
        checkGLError();

        GLUtils.texSubImage2D(GLES20.GL_TEXTURE_2D, 0, 0, 0, bitmap_);
        checkGLError();

        if (frame_ == 0) {
            copyBuffer();
        }
        frame_++;
    }

    @Override
    public void onSurfaceChanged(final GL10 gl, final int width, final int height) {
        GLES20.glViewport(0, 0, width, height);
        checkGLError();

        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap_, 0);
        checkGLError();
    }

    @Override
    public void onSurfaceCreated(final GL10 gl, final EGLConfig config) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_STENCIL_BUFFER_BIT);
        checkGLError();

        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        checkGLError();

        //Enable culling
        GLES20.glEnable(GLES20.GL_CULL_FACE);
        checkGLError();

        GLES20.glCullFace(GLES20.GL_BACK);
        checkGLError();

        GLES20.glFrontFace(GLES20.GL_CCW);
        checkGLError();

        //Create geometry and texCoords buffers
        final ByteBuffer bbVertices = ByteBuffer.allocateDirect(vertices.length << 2);
        bbVertices.order(ByteOrder.nativeOrder());
        floatBufferVertices_ = bbVertices.asFloatBuffer();
        floatBufferVertices_.put(vertices);
        floatBufferVertices_.position(0);

        final ByteBuffer byteBufferTexCoords = ByteBuffer.allocateDirect(texCoords.length << 2);
        byteBufferTexCoords.order(ByteOrder.nativeOrder());
        floatBufferTexture_ = byteBufferTexCoords.asFloatBuffer();
        floatBufferTexture_.put(texCoords);
        floatBufferTexture_.position(0);

        //Load shaders
        final int vertexShader = MainRenderer.loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
        final int fragmentShader = MainRenderer.loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);

        //Create Program
        final int shaderProgram = GLES20.glCreateProgram();
        checkGLError();

        if (shaderProgram == 0) {
            Log.e("PROGRAM SHADER", "Could not create program: ");
            Log.e("PROGRAM SHADER", GLES20.glGetProgramInfoLog(0));
            System.exit(0);
        }

        //Attach and link shaders to program
        GLES20.glAttachShader(shaderProgram, vertexShader);
        checkGLError();

        GLES20.glAttachShader(shaderProgram, fragmentShader);
        checkGLError();

        GLES20.glLinkProgram(shaderProgram);
        checkGLError();

        final int[] linkStatus = new int[1];
        GLES20.glGetProgramiv(shaderProgram, GLES20.GL_LINK_STATUS, linkStatus, 0);
        checkGLError();

        if (linkStatus[0] != GLES20.GL_TRUE) {
            Log.e("PROGRAM SHADER LOG", "Could not link program: ");
            Log.e("PROGRAM SHADER LOG", GLES20.glGetProgramInfoLog(shaderProgram));
            GLES20.glDeleteProgram(shaderProgram);
            System.exit(0);
        }

        //Enable Textures
        //GLES20.glEnable(GLES20.GL_TEXTURE_2D);
        checkGLError();

        final int[] textureHandle = new int[1];
        GLES20.glGenTextures(1, textureHandle, 0);
        if (textureHandle[0] == 0) {
            Log.e("Error loading texture.", "Error loading texture");
            System.exit(0);
        }


        // Bind to the texture in OpenGL
        GLES20.glUseProgram(shaderProgram);
        checkGLError();

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        checkGLError();

        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);
        checkGLError();

        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        checkGLError();

        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        checkGLError();

        //Bind Attributes
        final int positionAttrib = GLES20.glGetAttribLocation(shaderProgram, "vPosition");
        checkGLError();
        GLES20.glVertexAttribPointer(positionAttrib, 3, GLES20.GL_FLOAT, false, 0, floatBufferVertices_);
        checkGLError();

        GLES20.glEnableVertexAttribArray(positionAttrib);
        checkGLError();


        final int texCoordAttrib = GLES20.glGetAttribLocation(shaderProgram, "a_texCoord");
        checkGLError();
        GLES20.glVertexAttribPointer(texCoordAttrib, 2, GLES20.GL_FLOAT, false, 0, floatBufferTexture_);
        checkGLError();

        GLES20.glEnableVertexAttribArray(texCoordAttrib);
        checkGLError();

        //Bind Uniform
        final int textureUniform = GLES20.glGetUniformLocation(shaderProgram, "u_Texture");
        checkGLError();

        GLES20.glUniform1i(textureUniform, 0);
        checkGLError();
    }
}
